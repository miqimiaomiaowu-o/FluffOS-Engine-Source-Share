#!/usr/bin/env python3
import argparse
import hashlib
import shutil
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, "/opt/wdsf/gs")
import patch_double_magic_hit_wrapper_server as pak_helper


ROOT = Path("/opt/wdsf/gs")
LIB_LIVE = ROOT / "gs/pack_data/lib_gs32.pak"
ENTRY = "gs/daemons/status/god_book.o"
BASE_ENTRY = "gs/daemons/status/god_boo0.o"
SOURCE_ENTRY = "/gs/daemons/status/god_book.c"
SOURCE_REAL = ROOT / "gs/daemons/status/god_book.c"
OLD_PATH = b"/gs/daemons/status/god_book.c"
BASE_PATH = b"/gs/daemons/status/god_boo0.c"
MARKER = b"qingtian-god-book-damage-multi-v2-20260616"
MARKER_PREFIX = b"qingtian-god-book-damage-multi"


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_path(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def make_base_object(original: bytes) -> bytes:
    if len(OLD_PATH) != len(BASE_PATH):
        raise SystemExit("god_book base path length mismatch")
    if original[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit("live god_book.o bad header")
    count = original.count(OLD_PATH)
    if count < 1:
        raise SystemExit("live god_book.o original source path missing")
    base = original.replace(OLD_PATH, BASE_PATH)
    if OLD_PATH in base:
        raise SystemExit("old god_book path still present in base object")
    if BASE_PATH not in base:
        raise SystemExit("base god_boo0 path missing after replacement")
    return base


def load_base_object(current_god_book: bytes) -> bytes:
    if MARKER_PREFIX in current_god_book:
        base = pak_helper.decoded_entry(LIB_LIVE, BASE_ENTRY)
        if base[:16].hex() != "01000100000000000000000000000000":
            raise SystemExit("live god_boo0.o bad header")
        if MARKER_PREFIX in base:
            raise SystemExit("live god_boo0.o appears wrapped")
        return base
    return make_base_object(current_god_book)


def compile_wrapper(source: Path, work: Path) -> Path:
    target = SOURCE_REAL
    backup = None
    same_source = source.resolve() == target.resolve()
    if target.exists():
        backup = target.with_name(target.name + f".bak_god_book_damage_multi_{int(time.time())}")
        shutil.copy2(target, backup)
    target.parent.mkdir(parents=True, exist_ok=True)
    if not same_source:
        shutil.copy2(source, target)

    bin_vm = "/" + work.name + "/bin"
    try:
        subprocess.check_call([str(ROOT / "qingtian_daozu_wudi"), SOURCE_ENTRY, bin_vm], cwd=ROOT)
    finally:
        if backup:
            shutil.copy2(backup, target)
        elif target.exists() and not same_source:
            target.unlink()

    obj = ROOT / bin_vm.strip("/") / ENTRY
    if not obj.exists():
        raise SystemExit(f"compiled god_book wrapper missing: {obj}")
    if obj.read_bytes()[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit(f"compiled god_book wrapper bad header: {obj.read_bytes()[:16].hex()}")
    return obj


def check_wrapper(raw: bytes) -> None:
    if MARKER not in raw:
        raise SystemExit("god_book damage multi marker missing")
    for key in (
        b"/gs/daemons/status/god_boo0.c",
        b"affect_action",
        b"apply_status",
        b"qingtian_multi_god_book",
        b"all_book_skills",
        b"select_skills",
        b"effect_skill",
        b"xiuluoshu",
        b"nuji",
        b"xiangmozhan",
    ):
        if key not in raw:
            raise SystemExit(f"god_book wrapper missing key: {key!r}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Patch status/god_book.o so one damage event can trigger multiple god-book effects.")
    parser.add_argument("--source", required=True)
    parser.add_argument("--deploy", action="store_true")
    args = parser.parse_args()

    source = Path(args.source)
    if not source.exists():
        raise SystemExit(f"missing source: {source}")

    ts = time.strftime("%Y%m%d_%H%M%S")
    work = ROOT / f"qingtian_status_god_book_damage_multi_{ts}"
    work.mkdir(parents=True, exist_ok=True)

    wrapper_o = compile_wrapper(source, work)
    wrapper = wrapper_o.read_bytes()
    check_wrapper(wrapper)

    original = pak_helper.decoded_entry(LIB_LIVE, ENTRY)
    base = load_base_object(original)
    if MARKER_PREFIX in base:
        raise SystemExit("base unexpectedly contains god_book damage multi marker")

    (work / "god_book.wrapper.o").write_bytes(wrapper)
    (work / "god_boo0.base.o").write_bytes(base)
    candidate = work / "lib_gs32.god_book_damage_multi.pak"
    pak_helper.write_pak(
        LIB_LIVE,
        candidate,
        {
            ENTRY: wrapper,
            BASE_ENTRY: base,
        },
    )

    print(f"work={work}")
    print(f"wrapper_o={wrapper_o}")
    print(f"wrapper_sha={sha256_bytes(wrapper)}")
    print(f"base_sha={sha256_bytes(base)}")
    print(f"candidate={candidate}")
    print(f"candidate_sha={sha256_path(candidate)}")

    if args.deploy:
        backup = LIB_LIVE.with_name(f"lib_gs32.pak.bak_before_god_book_damage_multi_{ts}")
        shutil.copy2(LIB_LIVE, backup)
        shutil.copy2(candidate, LIB_LIVE)
        rollback = work / "rollback_god_book_damage_multi_no_restart.sh"
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
