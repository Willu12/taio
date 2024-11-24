{
  description = "Nix project environment with clang, cmake and tooling";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs";
  };

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" ];
      forAllSystems = f: nixpkgs.lib.genAttrs supportedSystems (system: f system);
    in
    {
      devShells = forAllSystems
        (system:
          let
            pkgs = import nixpkgs {
              inherit system;
              config.allowUnfree = true;
            };
          in
          {
            default = with pkgs; mkShell.override { stdenv = gcc12Stdenv; }
              {
                shellHook = ''
                  zellij
                '';
                packages = [
                  clang-tools
                  bear
                  cmake
                  cmake-format
                  zellij
                  pkg-config
                ];
                buildInputs = [
                  ncurses5 binutils
                ];
              };
          }
        );
    };
}
