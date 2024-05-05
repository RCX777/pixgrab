{
  inputs = {
    nixpkgs.url     = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        buildInputs' = with pkgs; [ gcc gnumake xorg.libX11 ];
        devtools = with pkgs; [ gdb valgrind ];

        target = "pixgrab";
      in
      {
        devShells.default = pkgs.mkShell {
          buildInputs = buildInputs' ++ devtools;
        };

        packages.default = pkgs.stdenv.mkDerivation {
          name = "${target}";
          src = self;
          buildInputs = buildInputs';
          buildPhase = ''
            make
          '';
          installPhase = ''
            mkdir -p $out/bin
            cp ${target} $out/bin/${target}
          '';
        };
      }
    );
}

