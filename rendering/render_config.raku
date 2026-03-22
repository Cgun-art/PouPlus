use v6;

my Str $render-engine  = 'DirectX';
my Str $physics-engine = 'BulletPhysics';
my Str $shading-type   = 'PBR';
my Str $texture-type   = 'PBR';

sub map-uv(Num $u, Num $v --> Hash) { { u => $u, v => $v } }
sub map-xy(Num $x, Num $y --> Hash) { { x => $x, y => $y } }
sub map-xz(Num $x, Num $z --> Hash) { { x => $x, z => $z } }

sub print-config() {
    say "Render Engine  : $render-engine";
    say "Physics Engine : $physics-engine";
    say "Shading Type   : $shading-type";
    say "Texture Type   : $texture-type";
}
