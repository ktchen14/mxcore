DOCKER_NAME = vector-test

.PHONY: docker interact

docker: Dockerfile
	docker build -t $(DOCKER_NAME) .

interact:
	docker run -it --rm -w /vector --volume $(realpath $(PWD)/..):/vector $(DOCKER_NAME)
