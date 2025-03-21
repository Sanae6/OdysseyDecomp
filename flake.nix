{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    systems.url = "github:nix-systems/default";
    rust-overlay.url = "github:oxalica/rust-overlay";
  };

  outputs = inputs:
    {
    }
    // inputs.flake-parts.lib.mkFlake {inherit inputs;} {
      systems = import inputs.systems;
      perSystem = {
        config,
        self',
        inputs',
        pkgs,
        system,
        ...
      }: let
        overlays = [(import inputs.rust-overlay)];
        pkgs = import inputs.nixpkgs {
          inherit system overlays;
          config = {
            allowUnfree = true;
            # allow python2 to be imported in order to patch old clang
            allowInsecurePredicate = pkg: with inputs.nixpkgs.lib; getName pkg == "python" && substring 0 3 (getVersion pkg) == "2.7";
          };
        };
        rustToolchain = pkgs.pkgsBuildHost.rust-bin.stable.latest.default;
        tools = import ./tools/nix pkgs;
        localPackages = tools.packages;
        shellInputs = rec {
          packages = with pkgs;
            [
              hactool
              localPackages.nx2elf

              cmake
              ninja
              llvmPackages_18.clang
              llvmPackages_18.libllvm
              ccache
              pkg-config

              rustToolchain
              (python3.withPackages (python-pkgs: [
                python-pkgs.capstone
                python-pkgs.colorama
                python-pkgs.cxxfilt
                python-pkgs.pyelftools
                python-pkgs.watchdog
                python-pkgs.python-Levenshtein
                python-pkgs.toml
              ]))
              openssl
              llvmPackages_18.libclang
              ncurses5
              ncurses6
            ];
          env = {
            LD_LIBRARY_PATH = "${pkgs.lib.makeLibraryPath packages}";
            LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
          };
        };
      in {
        _module.args.pkgs = import inputs.nixpkgs {
          config.allowUnfree = true;
        };
        packages =
          localPackages
          // {
            python2 = pkgs.python2;
            setup = pkgs.writeShellApplication {
              name = "setup";
              text = ''
                linkIfNotFound() {
                  if [ ! -f "$2" ] && [ ! -d "$2" ]; then
                    local dirflag=""
                    if [ -d "$1" ]; then
                      dirflag="--directory"
                    fi
                    ln -s $dirflag "$1" "$2"
                  fi
                }

                linkIfNotFound ${localPackages.viking}/bin/decompme tools/decompme
                linkIfNotFound ${localPackages.viking}/bin/check tools/check
                linkIfNotFound ${localPackages.viking}/bin/listsym tools/listsym
                linkIfNotFound ${localPackages.clang391} toolchain/clang-3.9.1  --directory
                linkIfNotFound ${localPackages.clang401} toolchain/clang-4.0.1 -d

                python3 tools/setup.py "$@"
              '';
              runtimeInputs = shellInputs.packages;
              runtimeEnv = shellInputs.env;
            };
          };
        formatter = pkgs.alejandra;
        devShells.default =
          pkgs.mkShell {
            buildInputs = shellInputs.packages;
          }
          // shellInputs.env;
      };
    };
}
