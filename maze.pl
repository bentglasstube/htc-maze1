#!/usr/bin/env perl

use strict;
use warnings;

package Node {

  sub new {
    my ($class, $x, $y) = @_;
    return bless [ $x, $y, 9999, 9999, undef ], $class;
  }

  sub x { return shift->[0]; }
  sub y { return shift->[1]; }

  sub gscore {
    my ($self, $value) = @_;
    $self->[2] = $value if @_ > 1;
    return $self->[2];
  }

  sub fscore {
    my ($self, $value) = @_;
    $self->[3] = $value if @_ > 1;
    return $self->[3];
  }

  sub parent {
    my ($self, $value) = @_;
    $self->[4] = $value if @_ > 1;
    return $self->[4];
  }

  sub equals {
    my ($self, $other) = @_;
    return $self->x == $other->x && $self->y == $other->y;
  }
};

package Maze {

  sub _cell_in_set {
    my ($cell, @set) = @_;
    foreach (@set) { return 1 if $_->equals($cell); }
    return 0;
  }

  sub new {
    my ($class, @lines) = @_;

    my $self = bless {}, $class;

    my $height = @lines;
    my $width  = length $lines[0];

    for my $y (0 .. $height - 1) {
      my $line = $lines[$y];
      for my $x (0 .. $width - 1) {
        my $cell = substr $line, $x, 1;

        if ($cell eq 'S') {
          $self->{sx} = $x;
          $self->{sy} = $y;
        } elsif ($cell eq 'F') {
          $self->{gx} = $x;
          $self->{gy} = $y;
        }

        $self->{cells}{$y}{$x} = Node->new($x, $y) unless $cell eq 'X';
      }
    }

    return $self;
  }

  sub start {
    my ($self) = @_; return $self->{cells}{ $self->{sy} }{ $self->{sx} };
  }

  sub goal {
    my ($self) = @_; return $self->{cells}{ $self->{gy} }{ $self->{gx} };
  }

  sub at {
    my ($self, $x, $y) = @_;
    return $self->{cells}{$y}{$x};
  }

  sub neighbors {
    my ($self, $cell) = @_;

    my @candidates = (
      [ $cell->x - 1, $cell->y ],
      [ $cell->x + 1, $cell->y ],
      [ $cell->x,     $cell->y - 1 ],
      [ $cell->x,     $cell->y + 1 ],
    );

    return grep { defined } map { $self->at($_->[0], $_->[1]) } @candidates;
  }

  sub heuristic {
    my ($self, $cell) = @_;

    my $dx = abs $cell->x - $self->goal->x;
    my $dy = abs $cell->y - $self->goal->y;

    return $dx + $dy;
  }

  sub astar {
    my ($self) = @_;

    my @closed = ();
    my @open   = ($self->start);

    $self->start->gscore(0);
    $self->start->fscore(0+ $self->heuristic($self->start));

    while (@open) {
      @open = sort { $a->fscore <=> $b->fscore } @open;
      my $current = shift @open;
      return 1 if $current->equals($self->goal);

      push @closed, $current;
      foreach my $neighbor ($self->neighbors($current)) {
        next if _cell_in_set($neighbor, @closed);
        my $gscore = $current->gscore + 1;
        if (not _cell_in_set($neighbor, @open) or $gscore < $neighbor->gscore) {
          $neighbor->parent($current);
          $neighbor->gscore($gscore);
          $neighbor->fscore($gscore + $self->heuristic($neighbor));
          push @open, $neighbor unless _cell_in_set($neighbor, @open);
        }
      }
    }

    return undef;
  }

  sub construct_path {
    my ($self) = @_;

    my $current = $self->goal;
    my @path    = ();

    while (not $current->equals($self->start)) {
      my $previous = $current->parent or last;

      my $dir = '';
      $dir = 'E' if $previous->x < $current->x;
      $dir = 'W' if $previous->x > $current->x;
      $dir = 'S' if $previous->y < $current->y;
      $dir = 'N' if $previous->y > $current->y;

      unshift @path, $dir;
      $current = $previous;
    }

    return @path;
  }
};

my @lines = <STDIN>;
chomp foreach @lines;
my $maze = Maze->new(@lines);

$maze->astar;

my @path = $maze->construct_path;
print "$_\n" for @path;
