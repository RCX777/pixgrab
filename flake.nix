{
  inputs = {
    nixpkgs.url     = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        buildInputs' = with pkgs; [ gcc xorg.libX11 ];
        devtools = with pkgs; [ gdb valgrind ];

        warningFlags = "-Wall -Wextra -pedantic";
        libs = "-lX11";
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = buildInputs' ++ devtools;
        };

        packages.default = pkgs.stdenv.mkDerivation rec {
          name = "pixgrab";
          src = self;
          buildInputs = buildInputs';
          buildPhase = ''
            gcc -o ${name} ${name}.c ${libs} ${warningFlags}
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp ${name} $out/bin/${name}
          '';
        };
      }
    );
}

