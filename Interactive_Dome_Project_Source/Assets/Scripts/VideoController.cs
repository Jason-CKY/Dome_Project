using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class VideoController : MonoBehaviour
{

    public GameController gameController;
    public UnityEngine.Video.VideoPlayer videoPlayer;
    public MeshRenderer planeRenderer;
    public UnityEngine.Video.VideoClip[] clips;

    private int currentIndex;

    void Start()
    {
        videoPlayer.loopPointReached += OnFinishPlaying;
    }

    void Update(){
        if(Input.GetKeyUp(KeyCode.Space)){
            OnFinishPlaying(videoPlayer);
        }

        if(currentIndex == 4 & videoPlayer.frame > 1428){
            videoPlayer.frame = 70;
        }
    }

    public void playVideo(int index){
        videoPlayer.clip = clips[index];
        currentIndex = index;
        videoPlayer.Play();
        StartCoroutine(display());
    }

    IEnumerator display(){
        yield return new WaitForSeconds(0.4f);
        planeRenderer.enabled = true;
    }
    
    void OnFinishPlaying(UnityEngine.Video.VideoPlayer player)
    {
        player.Stop();
        gameController.NextScene();
        planeRenderer.enabled = false;
    }
}
