#!/bin/bash
set -e

# -- Configuration ----------------------------------------------------------

PROTOC_VERSION="30.2"
INSTALL_DIR="$HOME/.local"
BIN_DIR="$INSTALL_DIR/bin"
PROTOC="$BIN_DIR/protoc"

PROTO_DIR="proto"
OUT_DIR="gen"

PB_RELEASES="https://github.com/protocolbuffers/protobuf/releases"

# -- Detect system architecture ---------------------------------------------

ARCH="$(uname -m)"

if [ "$ARCH" = "x86_64" ]; then
    PROTOC_ARCH="linux-x86_64"
elif [ "$ARCH" = "aarch64" ]; then
    PROTOC_ARCH="linux-aarch_64"
else
    echo "Unsupported architecture: $ARCH"
    exit 1
fi

# -- Ensure protoc is installed --------------------------------------------

if [ ! -x "$PROTOC" ]; then
    echo "protoc not found, installing locally..."

    ZIP="protoc-${PROTOC_VERSION}-${PROTOC_ARCH}.zip"
    URL="$PB_RELEASES/download/v${PROTOC_VERSION}/${ZIP}"

    mkdir -p "$INSTALL_DIR"
    curl -L -o "$ZIP" "$URL"
    unzip -o "$ZIP" -d "$INSTALL_DIR"
    rm "$ZIP"
else
    echo "protoc found: $PROTOC"
fi

export PATH="$BIN_DIR:$PATH"

# -- Ensure grpc_cpp_plugin exists -----------------------------------------

GRPC_CPP_PLUGIN="$(command -v grpc_cpp_plugin)"

if [ -z "$GRPC_CPP_PLUGIN" ]; then
    echo "grpc_cpp_plugin not found."
    echo "Install it using your system package manager:"
    echo "  Arch   : pacman -S grpc"
    echo "  Fedora : dnf install grpc-devel"
    echo "  Ubuntu : apt install grpc++ grpc-proto"
    exit 1
fi

echo "Using grpc_cpp_plugin: $GRPC_CPP_PLUGIN"


# -- Clean output directory --------------------------------------------------

if [ -d "$OUT_DIR" ]; then
    echo "Removing existing '$OUT_DIR/' directory"
    rm -rf "$OUT_DIR"
fi

# -- Generate C++ files from .proto files (using find) ---------------------

mkdir -p "$OUT_DIR"

PROTO_FILES="$(find "$PROTO_DIR" -type f -name '*.proto')"

if [ -z "$PROTO_FILES" ]; then
    echo "No .proto files found in $PROTO_DIR"
    exit 0
fi

for proto_file in $PROTO_FILES; do
    echo "Compiling: $proto_file"

    protoc \
        -I "$PROTO_DIR" \
        --cpp_out="$OUT_DIR" \
        --grpc_out="$OUT_DIR" \
        --plugin=protoc-gen-grpc="$GRPC_CPP_PLUGIN" \
        "$proto_file"
done

echo ""
echo "Done. Generated files are in '$OUT_DIR/'"
