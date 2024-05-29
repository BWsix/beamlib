vec2 spiralMap(int index) {
    float k = ceil((sqrt(float(index)) - 1.0) / 2.0);
    float t = 2.0 * k + 1.0;
    float m = pow(t, 2.0);
    t--;

    if (index >= int(m - t))
        return vec2(k - (m - float(index)), -k);
    else
        m -= t;

    if (index >= int(m - t))
        return vec2(-k, -k + (m - float(index)));
    else
        m -= t;

    if (index >= int(m - t))
        return vec2(-k + (m - float(index)), k);
    else
        return vec2(k, k - (m - float(index) - t));
}
