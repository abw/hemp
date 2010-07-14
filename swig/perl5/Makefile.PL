use ExtUtils::MakeMaker;

WriteMakefile(
    NAME        => 'hemp',
    VERSION     => '0.01',
    OBJECT      => 'hemp_wrap.o',
    LIBS        => ['-L../../library -lhemp'],
    INC         => '-I../../include',
    PREREQ_PM   => {
        Badger      => 0.06,
    },
    dist        => {
        COMPRESS    => 'gzip',
        SUFFIX      => 'gz',
    },
    test        => {
        TESTS       => 't/*.t',
    },
);

