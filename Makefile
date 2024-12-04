


format:
	@find src -type f \( -name "*.cpp" -o -name "*.h" -o -name "*.c" \) -exec clang-format -i {} +

