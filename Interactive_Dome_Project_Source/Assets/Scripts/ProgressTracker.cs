using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ProgressTracker : MonoBehaviour
{
    public GameObject[] dots;
    public Sprite[] dotSprites;
    private int completedCount = 0;

    public void Increment(int playerId){
        if(completedCount == 5) return;
        dots[completedCount].GetComponentInChildren<SpriteRenderer>().sprite = dotSprites[playerId];
        completedCount++;
    }

    public GameObject GetGameObject(){
        return gameObject;
    }

    public void Vanish(){
        iTween.ScaleTo(gameObject, iTween.Hash("scale", Vector3.zero, "time", 0.5, "oncomplete", "DestroySelf", "oncompletetarget", gameObject));
    }

    void DestroySelf(){
        Destroy(gameObject);
    }
}
