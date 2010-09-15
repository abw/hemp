package Hemp;

use 5.008;
use Badger;
use Badger::Class
    base => 'Dynaloader';

our $VERSION = 0.01;

require Dynaloader;
our @ISA = qw(DynaLoader);

bootstrap Hemp $VERSION;

1;

