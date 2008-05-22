my %table;

while (<>) {
    if (/('I','[^']+')/) {
        $table{$1} = 1;
    }
}

print "$_\n" for (keys %table);
