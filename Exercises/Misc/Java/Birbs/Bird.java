package java2;

public interface Bird {
    /**
     *
     * @param X : new X position
     * @param Y : new Y position
     * @return : travel time 
     */
    public float move(float X, float Y);
    public float getMoveSpeed();
    public float getLocationX();
    public float getLocationY();
}