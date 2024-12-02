{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  packages = [
    pkgs.texlive.combined.scheme-full
    (pkgs.python3.withPackages (python-pkgs: with python-pkgs; [
      pandas
      matplotlib
    ]))
  ];
}
