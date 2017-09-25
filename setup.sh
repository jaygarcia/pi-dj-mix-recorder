sudo apt-get install \
	git \
	build-essential \
	libgtk-3-dev	\
	libasound2-dev	\
	libsndfile1-dev \
	libalsaplayer-dev

## The following is to install PortAudio library. There is no
## working portaudio lib version that includes portaudio.h
## as of Ubuntu 17.04.

## There has to be a way to do this within CMake.
mkdir -p ./tmp/portaudio
cd ./tmp/portaudio
git clone git@github.com:jaygarcia/portaudio.git .
./configure
make
sudo make install
#ASSUMING everything goes perfectly!!
cd ../..
rm -rf ./tmp/portaudio
echo "Done"

