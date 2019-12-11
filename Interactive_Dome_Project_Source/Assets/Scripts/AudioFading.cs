using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public static class AudioFading
{
    public static IEnumerator FadeOut(AudioSource source, float duration)
    {
        float startVolume = source.volume;
 
        while (source.volume > 0)
        {
            source.volume -= startVolume * Time.deltaTime / duration;
            if(source.volume < 0.2) source.volume = 0;
            yield return null;
        }
 
        source.Stop();
        source.volume = startVolume;
    }
 
    public static IEnumerator FadeIn(AudioSource source, float duration)
    {
        float startVolume = 0.2f;
 
        source.volume = 0;
        source.Play();
 
        while (source.volume < 1.0f)
        {
            source.volume += startVolume * Time.deltaTime / duration;
            yield return null;
        }
 
        source.volume = 1f;
    }
}
