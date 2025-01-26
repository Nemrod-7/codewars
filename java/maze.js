
function pathFinder (maze) {

    if (maze.length < 2) return 0;

    const size = maze.indexOf('\n') ;
    const directions = [-1, 1, -size, size];

    const val = (x) => x - '0';

    var visit = Array(maze.length);
    var distc = Array(maze.length);
    var queue = [[0,0]];

    for (let i = 0; i < distc.length; i++) {
        distc[i] = Number.MAX_VALUE, visit[i] = false;
    }

    distc[0] = 0;

    while (true) {
        var dist = Number.MAX_VALUE, curr = 0;

        for (var i = 0; i < distc.length; i++) {
            if (distc[i] < dist && !visit[i]) {
                dist = distc[i], curr = i;
            }
        }

        console.log(dist, "[" , curr % size, Math.trunc(curr / size), "]");
        if (curr == maze.length - 1) return distc[curr];

        visit[curr] = true;

        for (const pos of directions ) {
            let next = pos + curr;

            if (next >= 0 && next < maze.length && !visit[next]) { 
                alt = distc[curr] + Math.abs( val(maze[curr]) - val(maze[next])) ;

                if (alt <= distc[next]) {
                    distc[next] = alt;
                }
            }
        }
    }

    return -1;
}

console.log("dist ", pathFinder(
`777000
007000
007000
007000
007000
007777`)) ; // 0

console.log("dist ", pathFinder(
`010
010
010`)) ; // 2



