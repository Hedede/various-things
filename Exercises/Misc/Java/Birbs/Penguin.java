package java2;

public class Penguin implements Bird {
    /*
        Location relative to origin.
    */
    private float LocationX;
    private float LocationY;
    
    /*
        Movement speed in km/h
    */
    private final float Speed;
    
    /*
        Default constructor
    */
    public Penguin()
    {
        LocationX   = 0.f;
        LocationY   = 0.f;
        Speed       = 6.f;
        
        BirdController.Instance.Register(this);
    }    
       
    /**
     * Constructs Penguin with initial position
     * @param X : initial X
     * @param Y : initial Y
     */
    public Penguin(float X, float Y)
    {
        LocationX   = X;
        LocationY   = Y;
        Speed       = 6.f;
        
        BirdController.Instance.Register(this);
    }   
    
    @Override public float move(float X, float Y)
    {
        double TimeTaken = Math.sqrt(Math.pow(X-LocationX,2) + Math.pow(Y-LocationY,2))/getMoveSpeed();
        LocationX = X;
        LocationY = Y;
        return (float) (TimeTaken); 
    }

    @Override public float getMoveSpeed()
    {
        return Speed;
    }
    
    @Override public float getLocationX()
    {
        return LocationX;
    }
    
    @Override public float getLocationY()
    {
        return LocationY;
    }
}
