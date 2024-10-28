{ pkgs }: {
  deps = [
    pkgs.nettools
    pkgs.unzip
    pkgs.wget
    pkgs.at
    pkgs.bashInteractive
    pkgs.nodePackages.bash-language-server
    pkgs.man
  ];
}