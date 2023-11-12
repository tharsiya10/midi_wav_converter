aubio_pitch_t {
    samplerate,
    pitch_type
    bufsize
    fvec buf

}

samplerate = 44100 Hz
hopSize = 256
winSize = 4096

aubio_pitch_t *
new_aubio_pitch (const char_t * pitch_mode,
    uint_t bufsize, uint_t hopsize, uint_t samplerate)
{
    aubio_pitch_t *p = AUBIO_NEW (aubio_pitch_t);
    aubio_pitch_type pitch_type;
    p->samplerate = samplerate;
    p->type = pitch_type;
    aubio_pitch_set_unit (p, "default");
    p->bufsize = bufsize;
    p->silence = DEFAULT_PITCH_SILENCE;
    p->conf_cb = NULL;
    switch (p->type) {
        case aubio_pitcht_yin:
            p->buf = new_fvec (bufsize);
            p->p_object = new_aubio_pitchyin (bufsize);
            if (!p->p_object) goto beach;
            p->detect_cb = aubio_pitch_do_yin;
            p->conf_cb = (aubio_pitch_get_conf_t)aubio_pitchyin_get_confidence;
            aubio_pitchyin_set_tolerance (p->p_object, 0.15);
            break;
}