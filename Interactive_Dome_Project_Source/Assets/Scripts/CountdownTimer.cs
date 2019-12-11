using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CountdownTimer : MonoBehaviour
{
    public GameController gameController;
    public GameObject spawn;

    public SpriteRenderer three;
    public SpriteRenderer two;
    public SpriteRenderer one;

    private AudioSource soundEffect;

    void Start(){
        soundEffect = GetComponent<AudioSource>();
    }

    public void StartCountdown(){
        iTween.ScaleFrom(gameObject, iTween.Hash("scale", Vector3.zero, "time", 0f, 
                        "delay", 2f, "oncomplete", "showThree"));
    }

    public void showThree(){
        three.enabled = true;
        iTween.ScaleTo(three.gameObject, iTween.Hash("scale", Vector3.zero, "time", 0.5f, 
                        "delay", 0.5f, "oncomplete", "showTwo", "oncompletetarget", gameObject));
        soundEffect.Play();
    }

    public void showTwo(){
        two.enabled = true;
        iTween.ScaleTo(two.gameObject, iTween.Hash("scale", Vector3.zero, "time", 0.5f, 
                        "delay", 0.5f, "oncomplete", "showOne", "oncompletetarget", gameObject));
        soundEffect.Play();
    }

    public void showOne(){
        one.enabled = true;
        iTween.ScaleTo(one.gameObject, iTween.Hash("scale", Vector3.zero, "time", 0.5f, 
                        "delay", 0.5f, "oncomplete", "DestroySelf", "oncompletetarget", gameObject));
        soundEffect.Play();
    }

    void DestroySelf(){
        gameController.CountdownEnded();
        Destroy(gameObject);
    }

}
