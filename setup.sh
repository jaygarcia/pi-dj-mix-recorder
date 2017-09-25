sudo apt-get install \
	git \
	build-essential \
	libgtk-3-dev	\
	libasound2-dev	\
	libsndfile1-dev \
	libalsaplayer-dev

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

