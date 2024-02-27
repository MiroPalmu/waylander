#!/bin/sh
# Uses comdb to generate compile_commands.json with headers
# Usage:
# ./.generate_compile_commands_json_with_headers.sh <build_root> <source_root>

compdb -p $1 list > $2/compile_commands.json \
    && echo "generated <source_root>/compile_commands.json" \
    || echo "failed to generate <source_root>/compile_commands.json"
