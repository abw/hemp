use 5.008;
use Badger Filesystem => 'Bin';
use ExtUtils::MakeMaker;
use Config;
    

# CCFLAGS
my $LIBRARY  = 'hemp';
my $INCDIR   = 'include';
my $LIBDIR   = 'library/.libs';     # weird shit that libtool does
my $HEMPDIR  = Bin->parent(1);
my (@INCDIRS, @LIBDIRS, @INCLUDES, @CCFLAGS, $LIBFLAGS, $INCFLAGS, $CCFLAGS);

# quick hack
@LIBDIRS  = ($HEMPDIR->dir($LIBDIR)->must_exist->definitive);
@INCDIRS  = ($HEMPDIR->dir($INCDIR)->must_exist->definitive);

$LIBFLAGS = join(" ", (map { "-L$_" } @LIBDIRS), "-l$LIBRARY");
$INCFLAGS = join(" ",  map { "-I$_" } @INCDIRS);
$CCFLAGS  = join(" ", @CCFLAGS);

print "INCFLAGS: $INCFLAGS\n";
print "LIBFLAGS: $LIBFLAGS\n";


WriteMakefile(
    NAME            => "Hemp",
    VERSION_FROM    => "lib/Hemp.pm",
    AUTHOR          => 'Andy Wardley <abw@cpan.org>',
    LICENSE         => "perl",
    CCFLAGS         => $CCFLAGS, 
    LIBS            => $LIBFLAGS,
    INC             => $INCFLAGS,
    PREREQ_PM       => {
        "Badger"        => 0.06,
        "Test::More"    => 0,
    },
);

