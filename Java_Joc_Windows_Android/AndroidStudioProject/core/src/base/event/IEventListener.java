package base.event;

/**
 * Created by adrin on 11/29/16.
 */

public interface IEventListener<E> {

    void onEvent(E event);

}
