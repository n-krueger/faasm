from os import makedirs
from os.path import exists
from subprocess import run

from invoke import task

from faasmcli.util.env import PROJ_ROOT, FAASM_BUILD_DIR

DEV_TARGETS = [
    "codegen_func",
    "codegen_shared_obj",
    "func_runner",
    "simple_runner",
    "tests",
]


@task
def cmake(ctx, clean=False, build="Debug"):
    """
    Configures the CMake build
    """
    if clean and exists(FAASM_BUILD_DIR):
        run("rm -rf {}/*".format(FAASM_BUILD_DIR), shell=True, check=True)

    if not exists(FAASM_BUILD_DIR):
        makedirs(FAASM_BUILD_DIR)

    cmd = [
        "cmake",
        "-GNinja",
        "-DCMAKE_BUILD_TYPE={}".format(build),
        "-DCMAKE_CXX_COMPILER=/usr/bin/clang++-10",
        "-DCMAKE_C_COMPILER=/usr/bin/clang-10",
        PROJ_ROOT,
    ]

    run(" ".join(cmd), shell=True, check=True, cwd=FAASM_BUILD_DIR)


@task
def tools(ctx, clean=False):
    """
    Builds all the targets commonly used for development
    """
    cmake(ctx, clean=clean)

    targets = " ".join(DEV_TARGETS)

    run(
        "cmake --build . --target {}".format(targets),
        cwd=FAASM_BUILD_DIR,
        shell=True,
        check=True,
    )


@task
def cc(ctx, target, clean=False):
    """
    Compiles the given CMake target
    """
    if clean:
        cmake(ctx, clean=True)

    if target == "all":
        target = ""
    else:
        target = "--target {}".format(target)

    run(
        "cmake --build . {}".format(target),
        cwd=FAASM_BUILD_DIR,
        shell=True,
        check=True,
    )
