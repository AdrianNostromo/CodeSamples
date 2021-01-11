package base.util;

import com.badlogic.gdx.Gdx;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;

// General math utility functions.
public class MathUtil {
    
    private static final Random generator = new Random();

    public MathUtil() {
        super();

        //void
    }

    public static int randInt(int min, int max) {
        return randInt(generator, min, max);
    }
    
    public static int randInt(Random generator, int min, int max) {
        return generator.nextInt((max - min) + 1) + min;
    }

    public static float dist(float aX, float aY, float bX, float bY) {
        float dist = (float)Math.sqrt((bX - aX) * (bX - aX) + (bY - aY) * (bY - aY));

        return dist;
    }
/*asd_x
    public static void shuffleArrayFisherYates(int[] ar) {
        // If running on Java 6 or older, use `new Random()` on RHS here
        Random rnd = ThreadLocalRandom.current();
        for (int i = ar.length - 1; i > 0; i--)
        {
            int index = rnd.nextInt(i + 1);
            // Simple swap
            int a = ar[index];
            ar[index] = ar[i];
            ar[i] = a;
        }
    }
*/
	public static String intToPaddedString(int value, int padding) {
		return String.format("%0" + padding + "d", value);
	}

	public static String cashCentsValueToString(int valueCents) {
		int dollars = (int)Math.floor(valueCents / 100);
		int cents = valueCents - dollars * 100;

		String ret = Integer.toString(dollars) + "." + intToPaddedString(cents, 2);

		return ret;
	}

	public static final String md5(final String s) {
		try {
			// Create MD5 Hash
			MessageDigest digest = MessageDigest
					.getInstance("MD5");
			digest.update(s.getBytes());
			byte messageDigest[] = digest.digest();

			// Create Hex String
			StringBuffer hexString = new StringBuffer();
			for (int i = 0; i < messageDigest.length; i++) {
				String h = Integer.toHexString(0xFF & messageDigest[i]);
				while (h.length() < 2)
					h = "0" + h;
				hexString.append(h);
			}
			return hexString.toString();

		} catch (NoSuchAlgorithmException e) {
			Gdx.app.debug("Error", "MD5 algorithm error occurred.");
		}

		return "";
	}

	public static float toRadians(float angdeg) {
		return angdeg / 180f * (float)Math.PI;
	}

	public static float toDegrees(float angrad) {
		return angrad * 180f / (float)Math.PI;
	}

	public static float angleDifferenceDegrees(float angStart, float angEnd) {
        float a = angEnd - angStart;
        a = customModulo((a + 180), 360) - 180;
        
        return a;
	}
    
    private static float customModulo(float a, float n) {
        return (float)(a - Math.floor(a/n) * n);
    }
    
}
