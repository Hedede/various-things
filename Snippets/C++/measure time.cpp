    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    function();
    high_resolution_clock::time_point t2 = high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();

    cout << duration;