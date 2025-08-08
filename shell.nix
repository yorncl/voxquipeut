# shell.nix
{
  pkgs ? import <nixpkgs> { },
}:
pkgs.mkShell {
  buildInputs = with pkgs; [
    pkg-config


    llvmPackages_latest.lldb
    llvmPackages_latest.libllvm
    llvmPackages_latest.libcxx
    llvmPackages_latest.clang-tools
    bear
    cmake
    gdb

    pkg-config
    glfw
    glm
    libGL
  ];
}

