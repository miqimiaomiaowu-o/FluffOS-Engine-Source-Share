#!/usr/bin/env python3
import argparse
import hashlib
import shutil
import struct
import subprocess
import sys
import time
from pathlib import Path


ROOT = Path("/opt/wdsf/gs")
LIB_LIVE = ROOT / "gs/pack_data/lib_gs32.pak"
TOOLS = ROOT / "codex_tools"
OLD_PATH = b"/gs/daemons/actions/double_magic_hit.c"
BASE_PATH = b"/gs/daemons/actions/double_magic_bak.c"
MARKER = b"fashu-lianji-double-magic-relay-no-register-20260615"
WRAPPER_MARKER_PREFIX = b"fashu-lianji-double-magic-wrapper"
ENTRY_SIZE = 0x40
MAGIC = 0x11223344
UNCOMPRESSED_FLAG = 0x80000000


def sha256_path(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def u32(buf: bytes, off: int) -> int:
    return struct.unpack_from("<I", buf, off)[0]


def write_u32(buf: bytearray, off: int, value: int) -> None:
    struct.pack_into("<I", buf, off, value & 0xFFFFFFFF)


def read_name(buf: bytes, off: int) -> str:
    return buf[off:off + 44].split(b"\x00", 1)[0].decode("utf-8", "replace")


def parse_table(buf: bytes, table_off: int, table_size: int, prefix: str = ""):
    if table_size % ENTRY_SIZE:
        raise SystemExit(f"bad table size: {table_size}")
    nodes = []
    for idx in range(table_size // ENTRY_SIZE):
        off = table_off + idx * ENTRY_SIZE
        name = read_name(buf, off + 20)
        if not name:
            continue
        flags = u32(buf, off)
        data_off = u32(buf, off + 4)
        packed_size = u32(buf, off + 8)
        size = u32(buf, off + 12)
        path = f"{prefix}/{name}" if prefix else name
        node = {
            "name": name,
            "path": path,
            "kind": "dir" if flags & 1 else "file",
            "flags": flags,
            "offset": data_off,
            "packed_size": packed_size,
            "size": size,
            "timestamp": u32(buf, off + 16),
            "children": [],
            "raw": b"",
        }
        if node["kind"] == "dir":
            node["children"] = parse_table(buf, data_off, packed_size, path)
        else:
            node["raw"] = bytes(buf[data_off:data_off + packed_size])
        nodes.append(node)
    return nodes


def find_node(nodes, path: str):
    path = path.strip("/")
    for node in nodes:
        if node["path"] == path:
            return node
        found = find_node(node.get("children", []), path)
        if found:
            return found
    return None


def ensure_dir(nodes, path: str):
    current = nodes
    prefix = ""
    for part in [p for p in path.strip("/").split("/") if p]:
        full = f"{prefix}/{part}" if prefix else part
        node = next((n for n in current if n["name"] == part), None)
        if node is None:
            node = {
                "name": part,
                "path": full,
                "kind": "dir",
                "flags": 1,
                "offset": 0,
                "packed_size": 0,
                "size": 0,
                "timestamp": int(time.time()),
                "children": [],
                "raw": b"",
            }
            current.append(node)
        if node["kind"] != "dir":
            raise SystemExit(f"not a dir: {full}")
        current = node["children"]
        prefix = full
    return current


def assign_offsets(nodes, cursor: int) -> int:
    for node in nodes:
        node["offset"] = cursor
        if node["kind"] == "dir":
            node["packed_size"] = len(node["children"]) * ENTRY_SIZE
            node["size"] = node["packed_size"]
            cursor += node["packed_size"]
            cursor = assign_offsets(node["children"], cursor)
        else:
            node["packed_size"] = len(node["raw"])
            if node["flags"] & UNCOMPRESSED_FLAG:
                node["size"] = len(node["raw"])
            cursor += len(node["raw"])
    return cursor


def table_bytes(nodes) -> bytes:
    out = bytearray(len(nodes) * ENTRY_SIZE)
    for idx, node in enumerate(nodes):
        off = idx * ENTRY_SIZE
        raw_name = node["name"].encode("utf-8")
        if len(raw_name) > 44:
            raise SystemExit(f"name too long: {node['name']}")
        write_u32(out, off, node["flags"])
        write_u32(out, off + 4, node["offset"])
        write_u32(out, off + 8, node["packed_size"])
        write_u32(out, off + 12, node["size"])
        write_u32(out, off + 16, node["timestamp"])
        out[off + 20:off + 20 + len(raw_name)] = raw_name
    return bytes(out)


def emit(nodes, chunks) -> None:
    chunks.append(table_bytes(nodes))
    for node in nodes:
        if node["kind"] == "dir":
            emit(node["children"], chunks)
        else:
            chunks.append(node["raw"])


def decoded_entry(input_pak: Path, path: str) -> bytes:
    sys.path.insert(0, str(TOOLS))
    import rebuild_etc_pak_lzss as pak

    buf = input_pak.read_bytes()
    root = pak.parse_table(buf, 16, pak.u32(buf, 4))
    node = pak.find_node(root, path)
    if not node:
        raise SystemExit(f"missing entry: {path}")
    return pak.decoded_entry(node)


def write_pak(input_pak: Path, output_pak: Path, replacements) -> None:
    sys.path.insert(0, str(TOOLS))
    import rebuild_etc_pak_lzss as pak

    buf = input_pak.read_bytes()
    if len(buf) < 16 or u32(buf, 0) != MAGIC:
        raise SystemExit("bad pak magic")
    root_len = u32(buf, 4)
    unknown_a = u32(buf, 8)
    unknown_b = u32(buf, 12)
    root = parse_table(buf, 16, root_len)

    for pak_path, raw in replacements.items():
        pak_path = pak_path.strip("/")
        node = find_node(root, pak_path)
        if node is None:
            parent_path, name = pak_path.rsplit("/", 1)
            parent = ensure_dir(root, parent_path)
            node = {
                "name": name,
                "path": pak_path,
                "kind": "file",
                "flags": UNCOMPRESSED_FLAG,
                "offset": 0,
                "packed_size": 0,
                "size": 0,
                "timestamp": int(time.time()),
                "children": [],
                "raw": b"",
            }
            parent.append(node)
        if node["kind"] != "file":
            raise SystemExit(f"not file entry: {pak_path}")
        node["raw"] = raw
        node["flags"] = (node["flags"] | UNCOMPRESSED_FLAG) & 0xFFFFFFFF
        node["timestamp"] = int(time.time())

    out_root_len = len(root) * ENTRY_SIZE
    assign_offsets(root, 16 + out_root_len)
    chunks = []
    emit(root, chunks)
    output_pak.write_bytes(struct.pack("<IIII", MAGIC, out_root_len, unknown_a, unknown_b) + b"".join(chunks))

    verify = output_pak.read_bytes()
    verify_root = parse_table(verify, 16, u32(verify, 4))
    for pak_path, raw in replacements.items():
        node = find_node(verify_root, pak_path)
        if not node:
            raise SystemExit(f"verify missing: {pak_path}")
        if pak.decoded_entry(node) != raw:
            raise SystemExit(f"verify mismatch: {pak_path}")


def make_base_object(original: bytes) -> bytes:
    if len(OLD_PATH) != len(BASE_PATH):
        raise SystemExit("base path length mismatch")
    if original[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit("original double_magic_hit.o bad header")
    count = original.count(OLD_PATH)
    if count < 1:
        raise SystemExit("original path not found in double_magic_hit.o")
    base = original.replace(OLD_PATH, BASE_PATH)
    if OLD_PATH in base:
        raise SystemExit("old path still present in base object")
    if BASE_PATH not in base:
        raise SystemExit("base path missing after replacement")
    return base


def load_base_object(current_double_magic_hit: bytes) -> bytes:
    if WRAPPER_MARKER_PREFIX in current_double_magic_hit:
        base = decoded_entry(LIB_LIVE, "gs/daemons/actions/double_magic_bak.o")
        if base[:16].hex() != "01000100000000000000000000000000":
            raise SystemExit("live double_magic_bak.o bad header")
        if WRAPPER_MARKER_PREFIX in base:
            raise SystemExit("live double_magic_bak.o appears wrapped")
        return base
    return make_base_object(current_double_magic_hit)


def compile_wrapper(source: Path, work: Path) -> Path:
    target = ROOT / "gs/daemons/actions/double_magic_hit.c"
    backup = None
    if target.exists():
        backup = target.with_name(target.name + f".bak_double_magic_wrapper_{int(time.time())}")
        shutil.copy2(target, backup)
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    bin_vm = "/" + work.name + "/bin"
    try:
        subprocess.check_call([str(ROOT / "qingtian_daozu_wudi"), "/gs/daemons/actions/double_magic_hit.c", bin_vm], cwd=ROOT)
    finally:
        if backup:
            shutil.copy2(backup, target)
        elif target.exists():
            target.unlink()
    obj = ROOT / bin_vm.strip("/") / "gs/daemons/actions/double_magic_hit.o"
    if not obj.exists():
        raise SystemExit(f"compiled wrapper missing: {obj}")
    if obj.read_bytes()[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit(f"compiled wrapper bad header: {obj.read_bytes()[:16].hex()}")
    return obj


def main() -> None:
    parser = argparse.ArgumentParser(description="Patch double_magic_hit wrapper into live GS lib.")
    parser.add_argument("--source", required=True)
    parser.add_argument("--deploy", action="store_true")
    args = parser.parse_args()

    source = Path(args.source)
    if not source.exists():
        raise SystemExit(f"missing source: {source}")

    ts = time.strftime("%Y%m%d_%H%M%S")
    work = ROOT / f"qingtian_double_magic_hit_wrapper_{ts}"
    work.mkdir(parents=True, exist_ok=True)

    wrapper_o = compile_wrapper(source, work)
    wrapper = wrapper_o.read_bytes()
    if MARKER not in wrapper:
        raise SystemExit("wrapper marker missing")
    if b"/gs/daemons/fashu_lianjid.c" not in wrapper:
        raise SystemExit("fashu daemon path missing in wrapper")
    if b"/gs/daemons/actions/double_magic_bak.c" not in wrapper:
        raise SystemExit("base daemon path missing in wrapper")

    original = decoded_entry(LIB_LIVE, "gs/daemons/actions/double_magic_hit.o")
    if MARKER in original:
        raise SystemExit("live double_magic_hit.o already appears wrapped")
    base = load_base_object(original)
    if WRAPPER_MARKER_PREFIX in base:
        raise SystemExit("base unexpectedly contains wrapper marker")

    (work / "double_magic_hit.wrapper.o").write_bytes(wrapper)
    (work / "double_magic_bak.base.o").write_bytes(base)
    candidate = work / "lib_gs32.double_magic_hit_wrapper.pak"
    write_pak(LIB_LIVE, candidate, {
        "gs/daemons/actions/double_magic_hit.o": wrapper,
        "gs/daemons/actions/double_magic_bak.o": base,
    })

    print(f"work={work}")
    print(f"wrapper_o={wrapper_o}")
    print(f"wrapper_sha={hashlib.sha256(wrapper).hexdigest()}")
    print(f"base_sha={hashlib.sha256(base).hexdigest()}")
    print(f"candidate={candidate}")
    print(f"candidate_sha={sha256_path(candidate)}")

    if args.deploy:
        backup = LIB_LIVE.with_name(f"lib_gs32.pak.bak_before_double_magic_hit_wrapper_{ts}")
        shutil.copy2(LIB_LIVE, backup)
        shutil.copy2(candidate, LIB_LIVE)
        rollback = work / "rollback_double_magic_hit_wrapper_no_restart.sh"
        rollback.write_text(
            "#!/bin/sh\n"
            f"cp -f {backup} {LIB_LIVE}\n"
            f"sha256sum {LIB_LIVE}\n",
            encoding="utf-8",
        )
        rollback.chmod(0o755)
        print(f"backup={backup}")
        print(f"rollback={rollback}")
        print(f"deployed_lib_sha={sha256_path(LIB_LIVE)}")
    else:
        print("deploy=not_performed")


if __name__ == "__main__":
    main()
