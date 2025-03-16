{
  lib,
  stdenv,
  fetchFromGitHub,
  lz4,
}:
stdenv.mkDerivation rec {
  pname = "nx2elf";
  version = "unstable-2021-11-21";

  src = fetchFromGitHub {
    owner = "leoetlino";
    repo = "nx2elf";
    rev = "b173627f79fa3f377924c7856b9cc6bc47f00df5";
    sha256 = "sha256-tbLwZuapwTfZXjlCM6iUURlYIU0UEM22GbZqlyLWSKw=";
  };

  buildInputs = [lz4];

  postPatch = ''
    # pkg-config is not supported, so we'll manually devendor lz4
    cp ${lz4.src}/lib/lz4.{h,c} .
  '';

  installPhase = ''
    mkdir -p $out/bin
    install -D nx2elf $out/bin/nx2elf
  '';
}
