{
  stdenv,
  lib,
  fetchurl,
  zlib,
  libgcc,
  libstdcxx5,
  ncurses5,
  python2,
  libxml2,
  libedit,
  autoPatchelfHook,
  clangUrls,
  ...
}:
stdenv.mkDerivation {
  name = "clang";

  src =
    if stdenv.isLinux && stdenv.isAarch64
    then fetchurl clangUrls.aarch64-linux
    else if stdenv.isLinux && stdenv.isx86_64
    then fetchurl clangUrls.x86_64-linux
    else abort "unsupported platform";

  sourceRoot = ".";

  nativeBuildInputs = [
    autoPatchelfHook
  ];

  buildInputs = [
    stdenv.cc.cc.lib
    zlib
    ncurses5
    python2
    libxml2
  ];

  postUnpack = ''
    cd clang+*
  '';

  installPhase = ''
    runHook preInstall
    mkdir -p $out
    cp -r * $out/
    chmod -R 755 $out

    ln -s ${libedit}/lib/libedit.so.0 $out/lib/libedit.so.2
    runHook postInstall
  '';
}
