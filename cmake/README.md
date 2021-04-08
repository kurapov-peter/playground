# Cmake python search test

docker build . --network host -t python-test   
docker run -it --name python-test python-test:latest bash  
   
cd playground/cmake && mkdir build && cd build
cmake .. && make

