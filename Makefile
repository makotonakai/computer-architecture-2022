.PHONY: build 

build:
		docker buildx create --name ostep --use
		docker buildx build --platform linux/amd64,linux/arm64 -t nyatsume/ostep-for-multiarch:latest --push .

clean:
		docker buildx rm ostep
