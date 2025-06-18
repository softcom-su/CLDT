package su.softcom.cldt.internal.core;

import java.nio.file.Path;

import su.softcom.cldt.core.IToolchain;

public class Toolchain implements IToolchain {

	private String name;
	private Path path;

	public Toolchain() {

	}

	public Toolchain(String name, Path path) {
		this.name = name;
		this.path = path;
	}

	@Override
	public void setName(String name) {
		this.name = name;
	}

	@Override
	public void setPath(Path path) {
		this.path = path;
	}

	@Override
	public String getName() {
		return this.name;
	}

	@Override
	public Path getPath() {
		return this.path;
	}

}