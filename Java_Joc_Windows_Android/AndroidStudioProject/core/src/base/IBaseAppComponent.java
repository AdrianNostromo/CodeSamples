package base;

/**
 * Created by Adrin on 6/23/2016.
 */
public interface IBaseAppComponent {

    void create();

    void appLoop(float deltaS, int deltaMS);

    void dispose();

}
