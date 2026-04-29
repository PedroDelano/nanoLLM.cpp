EIGEN_DIR := eigen-5.0.0
EIGEN_URL := https://gitlab.com/libeigen/eigen/-/archive/5.0.0/eigen-5.0.0.tar.gz

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -I $(EIGEN_DIR)

SRC := main.cpp
BIN := main

.PHONY: all check-eigen fetch-eigen clean

all: check-eigen $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

check-eigen:
	@if [ ! -d "$(EIGEN_DIR)" ]; then \
		echo "ERROR: '$(EIGEN_DIR)' not found."; \
		echo "Download Eigen 5.0.0 from:"; \
		echo "  $(EIGEN_URL)"; \
		echo "Or run: make fetch-eigen"; \
		exit 1; \
	fi

fetch-eigen:
	@echo "Downloading Eigen 5.0.0..."
	@curl -L -o eigen-5.0.0.tar.gz "$(EIGEN_URL)"
	@tar -xzf eigen-5.0.0.tar.gz
	@rm eigen-5.0.0.tar.gz
	@echo "Eigen extracted to $(EIGEN_DIR)/"

clean:
	rm -f $(BIN)
