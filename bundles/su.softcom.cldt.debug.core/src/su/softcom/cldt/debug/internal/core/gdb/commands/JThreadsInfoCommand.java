package su.softcom.cldt.debug.internal.core.gdb.commands;

import com.fasterxml.jackson.annotation.JsonInclude;
import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import java.util.List;
import java.util.Map;

import org.eclipse.core.runtime.Platform;

import su.softcom.cldt.debug.core.IGdbCommand;

public class JThreadsInfoCommand extends GDBCommand implements IGdbCommand {

	@JsonInclude(JsonInclude.Include.NON_NULL)
	public static class ThreadInfo {
		
		private String name;
	    private String reason;
	    private Map<Long, String> registers;
	    private Integer signal;
	    private Integer tid;
	    private Integer metype;
	    private List<Integer> medata;
	    private Long qaddr;
	    private List<JTIMemoryBlock> memory;
	    
	    // jThreadsInfo Memory Block
	    public class JTIMemoryBlock {
	        private Long address;
	        private String bytes;

	        public Long getAddress() {
	            return address;
	        }

	        public void setAddress(Long address) {
	            this.address = address;
	        }

	        public String getBytes() {
	            return bytes;
	        }

	        public void setBytes(String bytes) {
	            this.bytes = bytes;
	        }

	        @Override
	        public String toString() {
	            return "MemoryBlock{" +
	                    "address=" + address +
	                    ", bytes='" + bytes + '\'' +
	                    '}';
	        }
	    }
		
	    public String getName() {
	        return name;
	    }

	    public void setName(String name) {
	        this.name = name;
	    }

	    public String getReason() {
	        return reason;
	    }

	    public void setReason(String reason) {
	        this.reason = reason;
	    }

	    public Map<Long, String> getRegisters() {
	        return registers;
	    }

	    public void setRegisters(Map<Long, String> registers) {
	        this.registers = registers;
	    }

	    public Integer getSignal() {
	        return signal;
	    }

	    public void setSignal(Integer signal) {
	        this.signal = signal;
	    }

	    public Integer getTid() {
	        return tid;
	    }

	    public void setTid(Integer tid) {
	        this.tid = tid;
	    }

	    public Integer getMetype() {
	        return metype;
	    }

	    public void setMetype(Integer metype) {
	        this.metype = metype;
	    }

	    public List<Integer> getMedata() {
	        return medata;
	    }

	    public void setMedata(List<Integer> medata) {
	        this.medata = medata;
	    }

	    public Long getQaddr() {
	        return qaddr;
	    }

	    public void setQaddr(Long qaddr) {
	        this.qaddr = qaddr;
	    }

	    public List<JTIMemoryBlock> getMemory() {
	        return memory;
	    }

	    public void setMemory(List<JTIMemoryBlock> memory) {
	        this.memory = memory;
	    }
	    
	    private <K> String nullCheck(K value) {
	    	return value != null ? "" + value : "null";
	    }

	    @Override
	    public String toString() {
	        return "ThreadInfo{" +
	                "name='" + nullCheck(name) + '\'' +
	                ", reason='" + nullCheck(reason) + '\'' +
	                ", registers=" + nullCheck(registers) +
	                ", signal=" + nullCheck(signal) +
	                ", tid=" + nullCheck(tid) +
	                ", metype=" + nullCheck(metype) +
	                ", medata=" + nullCheck(medata) +
	                ", qaddr=" + nullCheck(qaddr) +
	                ", memory=" + nullCheck(memory) +
	                '}';
	    }

	}
	
	List<ThreadInfo> threadsInfo;
	
	public JThreadsInfoCommand() {
		super("jThreadsInfo");
	}
	
	private List<ThreadInfo> parseJson(String json) throws JsonProcessingException {
        ObjectMapper objectMapper = new ObjectMapper();
        return objectMapper.readValue(json, objectMapper.getTypeFactory().constructCollectionType(List.class, ThreadInfo.class));
    }
	
	public List<ThreadInfo> getThreadsInfo() {
		return threadsInfo;
	}

	@Override
	protected void doHandleResult(byte[] result) {
		try {
			this.threadsInfo = parseJson(new String(result));
		} catch (JsonProcessingException e) {
			e.printStackTrace();
		}	
		Platform.getLog(getClass()).info(threadsInfo.get(0).toString());		
	}

}
