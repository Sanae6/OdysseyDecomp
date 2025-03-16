pkgs: {
  packages = {
    nx2elf = pkgs.callPackage ./nx2elf.nix {};
    viking = pkgs.callPackage ./viking.nix {};
    clang391 = pkgs.callPackage ./clang.nix {
      clangUrls = {
        aarch64-linux = {
          url = "https://releases.llvm.org/3.9.1/clang+llvm-3.9.1-aarch64-linux-gnu.tar.xz";
          hash = "";
        };
        x86_64-linux = {
          url = "https://releases.llvm.org/3.9.1/clang+llvm-3.9.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz";
          hash = "sha256-mdH/1L6P0zMbTSR4rafubtNScpv+ShBwRQzbmjzo75s=";
        };
      };
    };
    clang401 = pkgs.callPackage ./clang.nix {
      clangUrls = {
        aarch64-linux = {
          url = "https://releases.llvm.org/4.0.1/clang+llvm-4.0.1-aarch64-linux-gnu.tar.xz";
          hash = "";
        };
        x86_64-linux = {
          url = "https://releases.llvm.org/4.0.1/clang+llvm-4.0.1-x86_64-linux-gnu-Fedora-25.tar.xz";
          hash = "sha256-cJ+dNLj8rKpdaabMyfevLFLaH/COalXoIVBK4Ro7oE8=";
        };
      };
    };
  };
}
