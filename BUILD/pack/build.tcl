package require psfgen

topology ./Topology.top

segment RESNAME {
    pdb packed.pdb
    first none
    last none
}

coordpdb ./packed.pdb RESNAME

writepsf ./START.psf
writepdb ./START.pdb