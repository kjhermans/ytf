#!/usr/bin/perl

my $textfile = shift @ARGV;
my $text = `cat $textfile`;

my $skipcapitals = shift @ARGV;

my $freq = {};

for (my $i=0; $i < length($text); $i++) {
  my $chr = ord(substr($text, $i, 1));
  next if ($skipcapitals && $chr >= ord('A') && $chr <= ord('Z'));
  if (!defined($freq->{$chr})) {
    $freq->{$chr} = 1;
  } else {
    ++($freq->{$chr});
  }
}

my $nbits = 5;
my $bit = 0;

my @list = sort { $freq->{$b} <=> $freq->{$a} } keys(%{$freq});

print "#define YTF_DICTIONARY \\\n/* { chr, prefix, encbitlen, encbits } */ \\\n";
for (my $bit=1; $bit < $nbits; $bit++) {
  my $roundnbits = 2 ** $bit;
  for (my $i=0; $i < $roundnbits; $i++) {
    my $chr = shift @list;
    my $lengthencoding = sprintf("%.2b", $bit-1);
    my $valueencoding = sprintf('%.' . $bit . 'b', $i);
    my $enc0 = bits2n("$lengthencoding");
    my $enc1 = bits2n("$valueencoding");
    print
      sprintf("{ 0x%.2x, 0x%.2x, %d, 0x%.2x }, " .
              "/* %s -> $lengthencoding $valueencoding " .
              "(freq $freq->{$chr}) */ \\\n"
        , $chr
        , $enc0
        , $bit 
        , $enc1
        , (($chr >= 32 && $chr < 127) ? chr($chr) : ' ')
      );
  }
}
print "{ 0, 0, 0, 0 }\n";

sub bits2n
{
  my $str = shift;
  my $n = 0;
  my $bitindex = 0;
  while (length($str)) {
    $str =~ s/(.)$//s;
    if ($1 eq '1') {
      $n |= (1 << $bitindex);
    }
    ++$bitindex;
  }
  return $n;
}

1;
