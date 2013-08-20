import java.awt.Color;
import java.awt.image.BufferedImage;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import javax.imageio.ImageIO;


public class ImageConverter {
	public static final String PATH="data/body";

	public static void main(String[] args)throws Exception{
		BufferedImage img=ImageIO.read(new File(PATH+".png"));
		float[] pix=new float[img.getWidth()*img.getHeight()*4];
		for(int y=0,i=0;y<img.getHeight();y++){
			for(int x=0;x<img.getWidth();x++,i+=4){
				Color c=new Color(img.getRGB(x, y));
				pix[i]=(float)Math.pow(c.getRed()/255.0f,2.2f);
				pix[i+1]=(float)Math.pow(c.getGreen()/255.0f,2.2f);
				pix[i+2]=(float)Math.pow(c.getBlue()/255.0f,2.2f);
				pix[i+3]=c.getAlpha()/255.0f;
			}
		}
		ByteBuffer buff = ByteBuffer.allocate(4*(2+pix.length));
		BufferedOutputStream bos=new BufferedOutputStream(new FileOutputStream(PATH+".tex"));
//		DataOutputStream dos=new DataOutputStream(bos);
		buff.order(ByteOrder.LITTLE_ENDIAN);

//		System.out.println(buff.putInt(img.getWidth()).getInt(0));
//		dos.writeInt(buff.putInt(img.getWidth()).getInt(0));
		buff.putInt(img.getWidth());
		buff.putInt(img.getHeight());

//		dos.writeInt(img.getHeight());
		for(int y=0,i=0;y<img.getHeight();y++){
			for(int x=0;x<img.getWidth();x++,i+=4){
				buff.putFloat(pix[i]);
				buff.putFloat(pix[i+1]);
				buff.putFloat(pix[i+2]);
				buff.putFloat(pix[i+3]);
//				dos.writeFloat(pix[i]);
//				dos.writeFloat(pix[i+1]);
//				dos.writeFloat(pix[i+2]);
//				dos.writeFloat(pix[i+3]);
			}
//			dos.flush();
		}
//		dos.flush();
//		dos.close();
		bos.write(buff.array());
		bos.flush();
		bos.close();
	}
}
