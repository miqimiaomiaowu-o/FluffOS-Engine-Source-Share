#!/usr/bin/env python3
import argparse
import hashlib
import shutil
import sys
import time
from pathlib import Path

sys.path.insert(0, "/opt/wdsf/gs")
import patch_double_magic_hit_wrapper_server as pak_helper


ROOT = Path("/opt/wdsf/gs")
LIB_LIVE = ROOT / "gs/pack_data/lib_gs32.pak"
STATUS_ENTRY = "gs/daemons/status/god_book.o"

PATCH_OFFSET = 0xC5D
ORIGINAL_WORD = bytes.fromhex("0000003b")
PATCHED_WORD = bytes.fromhex("cbff001f")
VERIFY_WINDOW_OFFSET = 0xB8D
VERIFY_WINDOW = bytes.fromhex(
    "fcff0039"
    "fbff0037"
    "01000016"
    "0300002f"
    "00000033"
    "5cff001f"
)
SUCCESS_TAIL_OFFSET = 0xC5D
SUCCESS_TAIL_ORIGINAL = bytes.fromhex(
    "0000003b"
    "00000049"
    "0000002e"
    "2aff001f"
)


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_path(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def patch_status(raw: bytes) -> bytes:
    if raw[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit("status/god_book.o bad object header")
    if raw.count(b"/gs/daemons/god_bookd.c") != 1:
        raise SystemExit("status/god_book.o is not on the native god_bookd path")
    if b"/gs/daemons/god_boo2d.c" in raw:
        raise SystemExit("status/god_book.o still points at god_boo2d")
    if raw[VERIFY_WINDOW_OFFSET:VERIFY_WINDOW_OFFSET + len(VERIFY_WINDOW)] != VERIFY_WINDOW:
        raise SystemExit("candidate removal loop byte window mismatch")
    if raw[SUCCESS_TAIL_OFFSET:SUCCESS_TAIL_OFFSET + len(SUCCESS_TAIL_ORIGINAL)] != SUCCESS_TAIL_ORIGINAL:
        raise SystemExit("success return byte window mismatch")
    patched = bytearray(raw)
    patched[PATCH_OFFSET:PATCH_OFFSET + 4] = PATCHED_WORD
    return bytes(patched)


def main() -> None:
    parser = argparse.ArgumentParser(description="Patch god_book affect_action success path to continue candidate loop.")
    parser.add_argument("--deploy", action="store_true")
    args = parser.parse_args()

    ts = time.strftime("%Y%m%d_%H%M%S")
    work = ROOT / f"qingtian_status_god_book_success_loop_{ts}"
    work.mkdir(parents=True, exist_ok=True)

    raw = pak_helper.decoded_entry(LIB_LIVE, STATUS_ENTRY)
    patched = patch_status(raw)

    (work / "status_god_book.before.o").write_bytes(raw)
    (work / "status_god_book.success_loop.o").write_bytes(patched)
    candidate = work / "lib_gs32.status_god_book_success_loop.pak"
    pak_helper.write_pak(LIB_LIVE, candidate, {STATUS_ENTRY: patched})

    print(f"work={work}")
    print(f"before_sha={sha256_bytes(raw)}")
    print(f"patched_sha={sha256_bytes(patched)}")
    print(f"candidate={candidate}")
    print(f"candidate_sha={sha256_path(candidate)}")
    print(f"patch_offset=0x{PATCH_OFFSET:x}")
    print(f"patch_word={PATCHED_WORD.hex()}")

    if args.deploy:
        backup = LIB_LIVE.with_name(f"lib_gs32.pak.bak_before_status_god_book_success_loop_{ts}")
        shutil.copy2(LIB_LIVE, backup)
        shutil.copy2(candidate, LIB_LIVE)
        rollback = work / "rollback_status_god_book_success_loop_no_restart.sh"
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
