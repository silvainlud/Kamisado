download:
	wget https://download.pytorch.org/libtorch/nightly/cpu/libtorch-shared-with-deps-latest.zip -O ./lib/data.zip
	unzip lib/data.zip -d lib/
	rm ./lib/data.zip