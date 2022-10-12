#! /bin/bash

if [ $# -lt 1 ] ; then
  echo "Usage: $0 np" ; exit 1
fi
np=$1

ARCH=full

####
#### Header
####
cat > simgrid_${ARCH}.xml <<EOF
<?xml version='1.0'?>
<!DOCTYPE platform SYSTEM "http://simgrid.gforge.inria.fr/simgrid/simgrid.dtd">

<platform version="4">

<zone id="first zone" routing="Full">
  <!-- the resources -->
EOF

####
#### Hosts
####
for h in `seq 1 ${np}` ; do
cat >>simgrid_${ARCH}.xml <<EOF
  <host id="host${h}" speed="1Mf"/>
EOF
done

####
#### Routing
####
cat >>simgrid_${ARCH}.xml <<EOF
  <link id="link1" bandwidth="125MBps" latency="100us"/>
  <!-- the routing: specify how the hosts are interconnected -->
EOF

for h in `seq 1 $(( np-1 ))` ; do
  for j in `seq $(( h+1 )) $(( np ))` ; do
    cat >>simgrid_${ARCH}.xml <<EOF
  <route src="host${h}" dst="host${j}"><link_ctn id="link1"/></route>
EOF
  done
done

####
#### Footer
####
cat >> simgrid_${ARCH}.xml <<EOF
</zone>

</platform>
EOF
