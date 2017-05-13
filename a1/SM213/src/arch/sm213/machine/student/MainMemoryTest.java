package arch.sm213.machine.student;

import machine.AbstractMainMemory;
import org.junit.Before;
import org.junit.Test;

import static org.junit.Assert.*;


/**
 * Created by Robin on 2017-01-11.
 */
public class MainMemoryTest {
    private MainMemory mem;
    private static final int SIZE = 256;
    private static final int SMALL_SIZE = 4;

    @Before
    public void runBefore() {

    }

    @Test
    public void isAccessAlingnedTest() {
        mem = new MainMemory(SIZE);
        // aligned
        assertTrue(mem.isAccessAligned(0x20, 2));
        assertTrue(mem.isAccessAligned(0x20, 4));
        assertTrue(mem.isAccessAligned(0x20, 8));

        // not aligned
        assertFalse(mem.isAccessAligned(0x11, 2));
        assertFalse(mem.isAccessAligned(0x11, 4));
        assertFalse(mem.isAccessAligned(0x11, 8));

    }

    @Test
    public void byteToIntegerTest() {
        mem = new MainMemory(SIZE);
        // byte is positive
        byte a = 0x12, b = 0x13, c = 0x14, d= 0x15;
        assertEquals(0x12131415, mem.bytesToInteger(a, b, c, d));

        // byte is negative
        byte p = 0xffffffff, q = 0xffffffff, m = 0xffffffff, n = 0xffffffff;
        assertEquals(-1, mem.bytesToInteger(p, q, m, n));
    }

    @Test
    public void integerToByteTest() {
        mem = new MainMemory(SIZE);
        // integer is positive
        assertArrayEquals(new byte[]{0x12, 0x13, 0x14, 0x15}, mem.integerToBytes(0x12131415));

        // integer is negative
        assertArrayEquals(new byte[]{0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff}, mem.integerToBytes(-1));
    }

    @Test (expected = AbstractMainMemory.InvalidAddressException.class)
    public void getTest() throws AbstractMainMemory.InvalidAddressException {
        // start at address 0
        mem = new MainMemory(SIZE);
        mem.set(0, new byte[]{0x12, 0x13, 0x14, 0x15});
        assertArrayEquals(new byte[]{0x12, 0x13, 0x14, 0x15}, mem.get(0, 4));

        // max length
        mem = new MainMemory(SMALL_SIZE);
        mem.set(0, new byte[]{0x12, 0x13});
        assertArrayEquals(new byte[]{0x12, 0x13, 0, 0}, mem.get(0, SMALL_SIZE));
        assertArrayEquals(new byte[]{0, 0}, mem.get(2, 2));

        // array out of range
        mem.get(2, SMALL_SIZE);
    }

    @Test (expected = AbstractMainMemory.InvalidAddressException.class)
    public void setTest() throws AbstractMainMemory.InvalidAddressException {
        mem = new MainMemory(SIZE);
        // start at address 0
        mem.set(0, new byte[]{0x12, 0x13, 0x14, 0x15});
        assertArrayEquals(mem.get(0, 4), new byte[]{0x12, 0x13, 0x14, 0x15});

        // start at middle
        mem = new MainMemory(SMALL_SIZE);
        mem.set(2, new byte[]{ 0x14, 0x15});
        assertArrayEquals(mem.get(2, 2), new byte[]{0x14, 0x15});

        // store data in the end, and rewrite the data there
        mem = new MainMemory(SMALL_SIZE);
        mem.set(2, new byte[]{0x12, 0x13});
        assertArrayEquals(mem.get(2, 2), new byte[]{0x12, 0x13});

        // array out of range
        mem.set(SMALL_SIZE, new byte[]{0x12, 0x13});

    }
}
