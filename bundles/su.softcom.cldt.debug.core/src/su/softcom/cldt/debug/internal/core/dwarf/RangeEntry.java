package su.softcom.cldt.debug.internal.core.dwarf;

public class RangeEntry {
    private final long startAddress;
    private final long endAddress;

    public RangeEntry(long startAddress, long endAddress) {
        this.startAddress = startAddress;
        this.endAddress = endAddress;
    }

    public long getStartAddress() {
        return startAddress;
    }

    public long getEndAddress() {
        return endAddress;
    }

    @Override
    public String toString() {
        return String.format("RangeEntry[start=0x%x, end=0x%x]", startAddress, endAddress);
    }
}
