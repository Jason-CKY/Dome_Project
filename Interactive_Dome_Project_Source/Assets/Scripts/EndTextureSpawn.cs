using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EndTextureSpawn : MonoBehaviour
{
    void Start()
    {
		iTween.ScaleFrom(transform.GetChild(0).gameObject, iTween.Hash("scale", Vector3.zero, "time", 3));
        GetComponentInChildren<SpriteRenderer>().enabled = true;
    }

    public void Vanish(){
        iTween.ScaleTo(transform.GetChild(0).gameObject, iTween.Hash("scale", Vector3.zero, "time", 0.5, "oncomplete", "DestroySelf", "oncompletetarget", gameObject));
    }

    void DestroySelf(){
        Destroy(gameObject);
    }
}
