#!/usr/bin/env python3
import argparse
import hashlib
import shutil
import subprocess
import time
from pathlib import Path

import patch_double_magic_hit_wrapper_server as pak_helper


ROOT = Path("/opt/wdsf/gs")
LIB_LIVE = ROOT / "gs/pack_data/lib_gs32.pak"
OLD_PATH = b"/gs/daemons/combat/receive.c"
BASE_PATH = b"/gs/daemons/combat/receivf.c"
MARKER = b"fashu-lianji-receive-transparent-v5-20260615"
WRAPPER_MARKER_PREFIX = b"fashu-lianji-receive-"


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_path(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def make_base_object(original: bytes) -> bytes:
    if len(OLD_PATH) != len(BASE_PATH):
        raise SystemExit("base path length mismatch")
    if original[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit("original receive.o bad header")
    count = original.count(OLD_PATH)
    if count < 1:
        raise SystemExit("original receive path not found")
    base = original.replace(OLD_PATH, BASE_PATH)
    if OLD_PATH in base:
        raise SystemExit("old receive path still present in base")
    if BASE_PATH not in base:
        raise SystemExit("base receive path missing")
    return base


def load_base_object(current_receive: bytes) -> bytes:
    if WRAPPER_MARKER_PREFIX in current_receive:
        base = pak_helper.decoded_entry(LIB_LIVE, "gs/daemons/combat/receivf.o")
        if base[:16].hex() != "01000100000000000000000000000000":
            raise SystemExit("live receivf.o bad header")
        if WRAPPER_MARKER_PREFIX in base:
            raise SystemExit("live receivf.o appears wrapped")
        return base
    return make_base_object(current_receive)


def compile_wrapper(source: Path, work: Path) -> Path:
    target = ROOT / "gs/daemons/combat/receive.c"
    backup = None
    if target.exists():
        backup = target.with_name(target.name + f".bak_receive_magic_wrapper_{int(time.time())}")
        shutil.copy2(target, backup)
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copy2(source, target)
    bin_vm = "/" + work.name + "/bin"
    try:
        subprocess.check_call(
            [str(ROOT / "qingtian_daozu_wudi"), "/gs/daemons/combat/receive.c", bin_vm],
            cwd=ROOT,
        )
    finally:
        if backup:
            shutil.copy2(backup, target)
        elif target.exists():
            target.unlink()
    obj = ROOT / bin_vm.strip("/") / "gs/daemons/combat/receive.o"
    if not obj.exists():
        raise SystemExit(f"compiled wrapper missing: {obj}")
    if obj.read_bytes()[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit(f"compiled wrapper bad header: {obj.read_bytes()[:16].hex()}")
    return obj


def main() -> None:
    parser = argparse.ArgumentParser(description="Patch receive.do_magic_hit wrapper into live GS lib.")
    parser.add_argument("--source", required=True)
    parser.add_argument("--deploy", action="store_true")
    args = parser.parse_args()

    source = Path(args.source)
    if not source.exists():
        raise SystemExit(f"missing source: {source}")

    ts = time.strftime("%Y%m%d_%H%M%S")
    work = ROOT / f"qingtian_receive_magic_hit_wrapper_{ts}"
    work.mkdir(parents=True, exist_ok=True)

    wrapper_o = compile_wrapper(source, work)
    wrapper = wrapper_o.read_bytes()
    if MARKER not in wrapper:
        raise SystemExit("wrapper marker missing")
    if BASE_PATH not in wrapper:
        raise SystemExit("base receive path missing in wrapper")

    original = pak_helper.decoded_entry(LIB_LIVE, "gs/daemons/combat/receive.o")
    if MARKER in original:
        raise SystemExit("live receive.o already appears wrapped")
    base = load_base_object(original)

    (work / "receive.wrapper.o").write_bytes(wrapper)
    (work / "receivf.base.o").write_bytes(base)
    candidate = work / "lib_gs32.receive_magic_hit_wrapper.pak"
    pak_helper.write_pak(LIB_LIVE, candidate, {
        "gs/daemons/combat/receive.o": wrapper,
        "gs/daemons/combat/receivf.o": base,
    })

    print(f"work={work}")
    print(f"wrapper_o={wrapper_o}")
    print(f"wrapper_sha={sha256_bytes(wrapper)}")
    print(f"base_sha={sha256_bytes(base)}")
    print(f"candidate={candidate}")
    print(f"candidate_sha={sha256_path(candidate)}")

    if args.deploy:
        backup = LIB_LIVE.with_name(f"lib_gs32.pak.bak_before_receive_magic_hit_wrapper_{ts}")
        shutil.copy2(LIB_LIVE, backup)
        shutil.copy2(candidate, LIB_LIVE)
        rollback = work / "rollback_receive_magic_hit_wrapper_no_restart.sh"
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
