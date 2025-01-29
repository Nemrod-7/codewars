
function pathFinder (maze) {

    if (maze.length < 2) return 0;

    const size = maze.indexOf('\n') ;
    const directions = [-1, 1, -size, size];

    const val = (x) => x - '0';

    var visit = Array(maze.length);
    var distc = Array(maze.length);
    var queue = [0];

    for (let i = 0; i < distc.length; i++) {
        distc[i] = 99999, visit[i] = false;
    }

    distc[0] = 0;

    while (true) {
        var dist = 99999, curr = -1;

        for (var i = 0; i < queue.length; i++) {
            exp = queue[i];

            if (distc[exp] < dist && !visit[exp]) {
                dist = distc[exp], curr = exp;
            }
            //process.stdout.write(i + ' ' + distc[i] + ' '+ dist + '\n');
        }
        console.log(curr);
        console.log(visit, distc);

        if (curr == maze.length - 1) return distc[curr];

        visit[curr] = true;

        for (const pos of directions ) {
            let next = pos + curr;

            if (next >= 0 && next < maze.length && !visit[next]) { 
                alt = distc[curr] + Math.abs( val(maze[curr]) - val(maze[next])) ;

                if (alt < distc[next]) {
                    distc[next] = alt;
                    queue.push(next);
                }
            }
        }
    }

    return -1;
}

//console.log("dist ", pathFinder(
//`777000
//007000
//007000
//007000
//007000
//007777`)) ; // 0

console.log("dist ", pathFinder(
`010
010
010`)) ; // 2



