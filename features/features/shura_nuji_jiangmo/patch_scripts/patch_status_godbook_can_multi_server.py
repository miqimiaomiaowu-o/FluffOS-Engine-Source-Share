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
SOURCE_ENTRY = "/gs/daemons/god_boo2d.c"
SOURCE_REAL = ROOT / "gs/daemons/god_boo2d.c"
PROXY_ENTRY = "gs/daemons/god_boo2d.o"
STATUS_ENTRY = "gs/daemons/status/god_book.o"
FORMULA_ENTRY = "gs/daemons/formula/skill_B_cast_on.o"
ACTION_ENTRIES = (
    "gs/daemons/actions/double_magic_hit.o",
)
OLD_PATH = b"/gs/daemons/god_bookd.c"
NEW_PATH = b"/gs/daemons/god_boo2d.c"
MARKER = b"qingtian-godbook-nuji-xiangmo-same-damage-v7-20260616"


def sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def sha256_path(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def compile_proxy(source: Path, work: Path) -> Path:
    target = SOURCE_REAL
    backup = None
    same_source = source.resolve() == target.resolve()
    if target.exists():
        backup = target.with_name(target.name + f".bak_godbook_status_multi_{int(time.time())}")
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

    obj = ROOT / bin_vm.strip("/") / PROXY_ENTRY
    if not obj.exists():
        raise SystemExit(f"compiled godbook status multi proxy missing: {obj}")
    if obj.read_bytes()[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit(f"compiled proxy bad header: {obj.read_bytes()[:16].hex()}")
    return obj


def patch_godbook_path(raw: bytes, entry: str, required: bool = True) -> bytes:
    if raw[:16].hex() != "01000100000000000000000000000000":
        raise SystemExit(f"{entry} bad object header")
    if len(OLD_PATH) != len(NEW_PATH):
        raise SystemExit("godbook daemon path length mismatch")
    if NEW_PATH in raw and OLD_PATH not in raw:
        return raw
    count = raw.count(OLD_PATH)
    if count == 0 and not required:
        return raw
    if count != 1:
        raise SystemExit(f"{entry} expected one god_bookd path, found {count}")
    patched = raw.replace(OLD_PATH, NEW_PATH)
    if OLD_PATH in patched:
        raise SystemExit(f"{entry} old god_bookd path still present")
    if NEW_PATH not in patched:
        raise SystemExit(f"{entry} proxy path missing after replacement")
    return patched


def check_proxy(raw: bytes) -> None:
    for key in (
        MARKER,
        b"/gs/daemons/god_bookd.c",
        b"can_godbook_affect",
        b"select_effect_skills",
        b"do_success_attach_god_book",
        b"xiuluoshu",
        b"nuji",
        b"xiangmozhan",
        b"QT_GB_FILL",
    ):
        if key not in raw:
            raise SystemExit(f"godbook status multi proxy missing key: {key!r}")


def main() -> None:
    parser = argparse.ArgumentParser(description="Patch status/god_book.o god_bookd calls through a narrow multi-trigger proxy.")
    parser.add_argument("--source", required=True)
    parser.add_argument("--deploy", action="store_true")
    args = parser.parse_args()

    source = Path(args.source)
    if not source.exists():
        raise SystemExit(f"missing source: {source}")

    ts = time.strftime("%Y%m%d_%H%M%S")
    work = ROOT / f"qingtian_status_godbook_can_multi_{ts}"
    work.mkdir(parents=True, exist_ok=True)

    proxy_o = compile_proxy(source, work)
    proxy = proxy_o.read_bytes()
    check_proxy(proxy)

    replacements = {
        PROXY_ENTRY: proxy,
    }

    status_raw = pak_helper.decoded_entry(LIB_LIVE, STATUS_ENTRY)
    status_patched = patch_godbook_path(status_raw, STATUS_ENTRY)
    replacements[STATUS_ENTRY] = status_patched

    formula_raw = pak_helper.decoded_entry(LIB_LIVE, FORMULA_ENTRY)
    formula_patched = patch_godbook_path(formula_raw, FORMULA_ENTRY)
    replacements[FORMULA_ENTRY] = formula_patched

    action_patched = {}
    for entry in ACTION_ENTRIES:
        raw = pak_helper.decoded_entry(LIB_LIVE, entry)
        action_patched[entry] = patch_godbook_path(raw, entry)
        replacements[entry] = action_patched[entry]

    (work / "god_boo2d.o").write_bytes(proxy)
    (work / "status_god_book.godbook_can_multi.o").write_bytes(status_patched)
    (work / "skill_B_cast_on.godbook_same_damage.o").write_bytes(formula_patched)
    for entry, raw in action_patched.items():
        (work / (entry.replace("/", "__") + ".godbook_same_damage.o")).write_bytes(raw)
    candidate = work / "lib_gs32.status_godbook_can_multi.pak"
    pak_helper.write_pak(LIB_LIVE, candidate, replacements)

    print(f"work={work}")
    print(f"proxy_o={proxy_o}")
    print(f"proxy_sha={sha256_bytes(proxy)}")
    print(f"status_sha={sha256_bytes(status_patched)}")
    print(f"formula_sha={sha256_bytes(formula_patched)}")
    for entry, raw in action_patched.items():
        print(f"{entry}_sha={sha256_bytes(raw)}")
    print(f"candidate={candidate}")
    print(f"candidate_sha={sha256_path(candidate)}")

    if args.deploy:
        backup = LIB_LIVE.with_name(f"lib_gs32.pak.bak_before_status_godbook_can_multi_{ts}")
        shutil.copy2(LIB_LIVE, backup)
        shutil.copy2(candidate, LIB_LIVE)
        rollback = work / "rollback_status_godbook_can_multi_no_restart.sh"
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
