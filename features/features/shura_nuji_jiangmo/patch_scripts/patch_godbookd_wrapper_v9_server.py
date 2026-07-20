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
SOURCE_ENTRY = "/gs/daemons/god_bookd.c"
SOURCE_REAL = ROOT / "gs/daemons/god_bookd.c"
WRAPPER_ENTRY = "gs/daemons/god_bookd.o"
BASE_ENTRY = "gs/daemons/god_boo4d.o"
STATUS_ENTRY = "gs/daemons/status/god_book.o"
OLD_STATUS_PATH = b"/gs/daemons/god_boo2d.c"
NATIVE_STATUS_PATH = b"/gs/daemons/god_bookd.c"
ORIG_GODBOOKD_PATH = b"/gs/daemons/god_bookd.c"
BASE_GODBOOKD_PATH = b"/gs/daemons/god_boo4d.c"
MARKER = b"qingtian-godbookd-wrapper-nuji-xiangmo-v9-20260616"


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_path(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def compile_wrapper(source: Path, work: Path) -> Path:
    backup = None
    same_source = source.resolve() == SOURCE_REAL.resolve()
    if SOURCE_REAL.exists():
        backup = SOURCE_REAL.with_name(SOURCE_REAL.name + f".bak_godbookd_wrapper_v9_{int(time.time())}")
        shutil.copy2(SOURCE_REAL, backup)
    SOURCE_REAL.parent.mkdir(parents=True, exist_ok=True)
    if not same_source:
        shutil.copy2(source, SOURCE_REAL)

    bin_vm = "/" + work.name + "/bin"
    try:
        subprocess.check_call([str(ROOT / "qingtian_daozu_wudi"), SOURCE_ENTRY, bin_vm], cwd=ROOT)
    finally:
        if backup:
            shutil.copy2(backup, SOURCE_REAL)
        elif SOURCE_REAL.exists() and not same_source:
            SOURCE_REAL.unlink()

    obj = ROOT / bin_vm.strip("/") / WRAPPER_ENTRY
    if not obj.exists():
        raise SystemExit(f"compiled godbookd wrapper missing: {obj}")
    if obj.read_bytes()[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit(f"compiled wrapper bad header: {obj.read_bytes()[:16].hex()}")
    return obj


def patch_base_object(raw: bytes) -> bytes:
    if len(ORIG_GODBOOKD_PATH) != len(BASE_GODBOOKD_PATH):
        raise SystemExit("base godbookd path length mismatch")
    if raw[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit("native god_bookd.o bad header")
    patched = raw.replace(ORIG_GODBOOKD_PATH, BASE_GODBOOKD_PATH)
    if patched.count(BASE_GODBOOKD_PATH) < 1:
        raise SystemExit("base godboo4d path missing")
    return patched


def patch_status_object(raw: bytes) -> bytes:
    if raw[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit("status/god_book.o bad header")
    if len(OLD_STATUS_PATH) != len(NATIVE_STATUS_PATH):
        raise SystemExit("status path length mismatch")
    if OLD_STATUS_PATH in raw:
        patched = raw.replace(OLD_STATUS_PATH, NATIVE_STATUS_PATH)
    else:
        patched = raw
    if OLD_STATUS_PATH in patched:
        raise SystemExit("status/god_book.o still points at god_boo2d")
    if NATIVE_STATUS_PATH not in patched:
        raise SystemExit("status/god_book.o native path missing")
    return patched


def check_wrapper(raw: bytes) -> None:
    for key in (
        MARKER,
        b"/gs/daemons/god_boo4d.c",
        b"do_success_attach_god_book",
        b"select_effect_skills",
        b"is_rate_satisfy",
        b"nuji",
        b"xiangmozhan",
        b"QT_GBD_V9",
    ):
        if key not in raw:
            raise SystemExit(f"godbookd wrapper missing key: {key!r}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Patch god_bookd itself through a fixed-signature wrapper.")
    parser.add_argument("--source", required=True)
    parser.add_argument("--deploy", action="store_true")
    args = parser.parse_args()

    source = Path(args.source)
    if not source.exists():
        raise SystemExit(f"missing source: {source}")

    ts = time.strftime("%Y%m%d_%H%M%S")
    work = ROOT / f"qingtian_godbookd_wrapper_v9_{ts}"
    work.mkdir(parents=True, exist_ok=True)

    native_godbookd = pak_helper.decoded_entry(LIB_LIVE, WRAPPER_ENTRY)
    base = patch_base_object(native_godbookd)
    status = patch_status_object(pak_helper.decoded_entry(LIB_LIVE, STATUS_ENTRY))

    wrapper_o = compile_wrapper(source, work)
    wrapper = wrapper_o.read_bytes()
    check_wrapper(wrapper)

    (work / "god_bookd.wrapper.o").write_bytes(wrapper)
    (work / "god_boo4d.native_base.o").write_bytes(base)
    (work / "status_god_book.native_path.o").write_bytes(status)
    candidate = work / "lib_gs32.godbookd_wrapper_v9.pak"
    pak_helper.write_pak(
        LIB_LIVE,
        candidate,
        {
            WRAPPER_ENTRY: wrapper,
            BASE_ENTRY: base,
            STATUS_ENTRY: status,
        },
    )

    print(f"work={work}")
    print(f"wrapper_o={wrapper_o}")
    print(f"wrapper_sha={sha256_bytes(wrapper)}")
    print(f"base_sha={sha256_bytes(base)}")
    print(f"status_sha={sha256_bytes(status)}")
    print(f"candidate={candidate}")
    print(f"candidate_sha={sha256_path(candidate)}")

    if args.deploy:
        backup = LIB_LIVE.with_name(f"lib_gs32.pak.bak_before_godbookd_wrapper_v9_{ts}")
        shutil.copy2(LIB_LIVE, backup)
        shutil.copy2(candidate, LIB_LIVE)
        rollback = work / "rollback_godbookd_wrapper_v9_no_restart.sh"
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
