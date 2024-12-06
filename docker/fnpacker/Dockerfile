FROM golang:1.21-bullseye

ENV GOMEMLIMIT=2GiB

RUN apt update \
  && apt install -y tini

COPY ./fnpacker /sesemi/fnpacker

RUN cd /sesemi/fnpacker \
  && go mod tidy \
  && CGO_ENABLED=1 GOOS=linux go build -o /fnpacker /sesemi/fnpacker

CMD ["/fnpacker", "-port", "7310"]
