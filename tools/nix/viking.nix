{
  lib,
  fetchFromGitHub,
  rustPlatform,
  pkg-config,
  openssl,
  libclang,
  clangStdenv,
}:
rustPlatform.buildRustPackage.override {
  stdenv = clangStdenv;
} rec {
  pname = "viking";
  version = "12.1.1";

  repo = fetchFromGitHub {
    owner = "open-ead";
    repo = "nx-decomp-tools";
    rev = "80ad085";
    hash = "sha256-eRD2L6f5UE8zpF+G3qdbr4Tv1z3cCv0zlXe/tRwfH0U=";
  };

  src = "${repo}/viking";

  nativeBuildInputs = [
    pkg-config
  ];

  buildInputs = [
    openssl
    libclang
  ];

  postPatch = ''
    cat ${./capstone-remove-doc.patch} | patch -s -p0 /build/cargo-vendor-dir/capstone-0.11.0/src/lib.rs
  '';

  cargoLock = {
    lockFile = "${src}/Cargo.lock";
    outputHashes = {
      "capstone-0.11.0" = "sha256-gq7nXdonQ9IghYxjbLo8aEeII4dnITTYb2+GQxflDlM=";
    };
  };

  LIBCLANG_PATH = "${libclang.lib.outPath}/lib";
}
