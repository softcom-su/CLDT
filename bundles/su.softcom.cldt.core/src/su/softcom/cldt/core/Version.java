package su.softcom.cldt.core;

import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Представляет версию инструмента в формате {@code major.minor.patch-suffix},
 * например, {@code 1.2.3-beta}.
 * <p>
 * Версия сравнивается по {@code major}, затем {@code minor}, затем
 * {@code patch}, затем по {@code suffix} (лексикографически). Суффикс может
 * быть {@code null}.
 * </p>
 * Примеры допустимых строк: {@code "1"}, {@code "1.2"}, {@code "1.2.3"},
 * {@code "1.2.3-beta"}.
 *
 */
public class Version implements Comparable<Version> {

	private static final Pattern PATTERN = Pattern.compile("^(\\d+)" //$NON-NLS-1$
			+ "(?:\\.(\\d+))?" //$NON-NLS-1$
			+ "(?:\\.(\\d+))?" //$NON-NLS-1$
			+ "(?:-(.+))?$" //$NON-NLS-1$
	);

	private final int major;
	private final int minor;
	private final int patch;
	private final String suffix;

	/**
	 * Создаёт версию по строке формата {@code major[.minor[.patch]][-suffix]}.
	 * 
	 * @param str строка, представляющая версию
	 * @throws IllegalArgumentException если строка не соответствует формату
	 */
	public Version(String str) {
		Matcher m = PATTERN.matcher(str.trim());
		if (!m.matches()) {
			throw new IllegalArgumentException(String.format("Cannot parse version string: \"%s\"", str));
		}
		major = Integer.parseInt(m.group(1));
		minor = m.group(2) != null ? Integer.parseInt(m.group(2)) : -1;
		patch = m.group(3) != null ? Integer.parseInt(m.group(3)) : -1;
		suffix = m.group(4) != null ? m.group(4).trim() : null;

	}

	/**
	 * Создаёт версию по компонентам.
	 * 
	 * @param major  старшая версия
	 * @param minor  минорная версия
	 * @param patch  патч-версия
	 * @param suffix суффикс (может быть null)
	 */
	public Version(int major, int minor, int patch, String suffix) {
		this.major = major;
		this.minor = minor;
		this.patch = patch;
		this.suffix = suffix;
	}

	/**
	 * Старшая версия (major).
	 */
	public int major() {
		return major;
	}

	/**
	 * Минорная версия (minor).
	 */
	public int minor() {
		return minor;
	}

	/**
	 * Патч-версия (patch).
	 */
	public int patch() {
		return patch;
	}

	/**
	 * Суффикс (например, "beta", "rc1").
	 * 
	 * @return суффикс или {@code null}
	 */
	public String suffix() {
		return suffix;
	}

	@Override
	public boolean equals(Object o) {
		if (this == o)
			return true;
		if (!(o instanceof Version other))
			return false;
		return major == other.major && minor == other.minor && patch == other.patch
				&& Objects.equals(suffix, other.suffix);
	}

	@Override
	public int hashCode() {
		return Objects.hash(major, minor, patch, suffix);
	}

	/**
	 * Сравнивает версии
	 */
	@Override
	public int compareTo(Version o) {
		int cmp = Integer.compare(major, o.major);
		if (cmp != 0)
			return cmp;
		cmp = Integer.compare(minor, o.minor);
		if (cmp != 0)
			return cmp;
		cmp = Integer.compare(patch, o.patch);
		if (cmp != 0)
			return cmp;
		if (suffix == null && o.suffix == null)
			return 0;
		if (suffix == null)
			return -1;
		if (o.suffix == null)
			return 1;
		return suffix.compareTo(o.suffix);
	}

	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(major);
		if (minor != 0 || patch != 0)
			sb.append('.').append(minor);
		if (patch != 0)
			sb.append('.').append(patch);
		if (suffix != null)
			sb.append('-').append(suffix);
		return sb.toString();
	}
}
