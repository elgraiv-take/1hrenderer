import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;

import javax.imageio.ImageIO;

public class BRDFConverter {
	public static final String PATH="data/brdf";

	public static void main(String[] args) throws Exception {
		BufferedImage img = ImageIO.read(new File(PATH + ".png"));
		float[] pix = new float[img.getWidth() * img.getHeight() * 3];
		for (int y = 0, i = 0; y < img.getHeight(); y++) {
			for (int x = 0; x < img.getWidth(); x++, i += 3) {
				Color c = new Color(img.getRGB(x, y));
				pix[i] = (float)Math.pow((c.getRed() / 255.0f),2.2);
				pix[i + 1] = (float)Math.pow((c.getGreen() / 255.0f),2.2);
				pix[i + 2] = (float)Math.pow((c.getBlue() / 255.0f),2.2);
			}
		}
		PrintWriter pw=new PrintWriter(new BufferedWriter(new FileWriter(PATH+".brdf")));
		for (int y = 0, i = 0; y < img.getHeight(); y++) {
			for (int x = 0; x < img.getWidth(); x++, i += 3) {
				pw.println(x+","+y+","+pix[i]+","+pix[i+1]+","+pix[i+2]);
			}
		}
		pw.flush();
		pw.close();

	}
}
