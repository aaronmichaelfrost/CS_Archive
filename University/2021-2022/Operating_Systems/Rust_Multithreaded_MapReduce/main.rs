/*
*   Aaron Frost
*   CS344 Multi-threaded Map Reduce in Rust
*   12/4/2021
*/


use std::env; // to get arugments passed to the program
use std::thread;

/*
* Print the number of partitions and the size of each partition
* @param vs A vector of vectors
*/
fn print_partition_info(vs: &Vec<Vec<usize>>){
    println!("Number of partitions = {}", vs.len());
    for i in 0..vs.len(){
        println!("\tsize of partition {} = {}", i, vs[i].len());
    }
}

/*
* Create a vector with integers from 0 to num_elements -1
* @param num_elements How many integers to generate
* @return A vector with integers from 0 to (num_elements - 1)
*/
fn generate_data(num_elements: usize) -> Vec<usize>{
    let mut v : Vec<usize> = Vec::new();
    for i in 0..num_elements {
        v.push(i);
    }
    return v;
}

/*
* Partition the data in the vector v into 2 vectors
* @param v Vector of integers
* @return A vector that contains 2 vectors of integers

*/
fn partition_data_in_two(v: &Vec<usize>) -> Vec<Vec<usize>>{
    let partition_size = v.len() / 2;
    // Create a vector that will contain vectors of integers
    let mut xs: Vec<Vec<usize>> = Vec::new();

    // Create the first vector of integers
    let mut x1 : Vec<usize> = Vec::new();
    // Add the first half of the integers in the input vector to x1
    for i in 0..partition_size{
        x1.push(v[i]);
    }
    // Add x1 to the vector that will be returned by this function
    xs.push(x1);

    // Create the second vector of integers
    let mut x2 : Vec<usize> = Vec::new();
    // Add the second half of the integers in the input vector to x2
    for i in partition_size..v.len(){
        x2.push(v[i]);
    }
    // Add x2 to the vector that will be returned by this function
    xs.push(x2);
    // Return the result vector
    xs
}

/*
* Sum up the all the integers in the given vector
* @param v Vector of integers
* @return Sum of integers in v
*/
fn map_data(v: &Vec<usize>) -> usize{
    let mut sum = 0;
    for i in v{
        sum += i;
    }
    sum
}

/*
* Sum up the all the integers in the given vector
* @param v Vector of integers
* @return Sum of integers in v
*/
fn reduce_data(v: &Vec<usize>) -> usize{
    let mut sum = 0;
    for i in v{
        sum += i;
    }
    sum
}

/*
* A single threaded map-reduce program
*/
fn main() {

    // Use std::env to get arguments passed to the program
    let args: Vec<String> = env::args().collect();
    if args.len() != 3 {
        println!("ERROR: Usage {} num_partitions num_elements", args[0]);
        return;
    }
    let num_partitions : usize = args[1].parse().unwrap();
    let num_elements : usize = args[2].parse().unwrap();
    if num_partitions < 1{
      println!("ERROR: num_partitions must be at least 1");
        return;
    }
    if num_elements < num_partitions{
        println!("ERROR: num_elements cannot be smaller than num_partitions");
        return;
    }

    // Generate data.
    let v = generate_data(num_elements);

    // PARTITION STEP: partition the data into 2 partitions
    let xs = partition_data_in_two(&v);

    // Print info about the partitions
    print_partition_info(&xs);

    let mut intermediate_sums : Vec<usize> = Vec::new();

    // MAP STEP: Process each partition

    // Create a thread for each vector
    let mut thread_handles: Vec<std::thread::JoinHandle<usize>> = Vec::with_capacity(2);
    for part in xs {
        thread_handles.push(thread::spawn(move || map_data(&part)));
    }

    // Complete the computations and load into array
    for thread in thread_handles{
        intermediate_sums.push(thread.join().unwrap());
    }

    // Print the vector with the intermediate sums
    println!("Intermediate sums = {:?}", intermediate_sums);

    // REDUCE STEP: Process the intermediate result to produce the final result
    let sum = reduce_data(&intermediate_sums);
    println!("Sum = {}", sum);

    // Create new
    let mut intermediate_sums : Vec<usize> = Vec::new();

    let v2 = partition_data(num_partitions, &v);
    print_partition_info(&v2);

    // Create a thread for each vector
    let mut thread_handles: Vec<std::thread::JoinHandle<usize>> = Vec::with_capacity(num_partitions);
    for part in v2 {
        thread_handles.push(thread::spawn(move || map_data(&part)));
    }

    // Complete the computations and load into array
    for thread in thread_handles{
        intermediate_sums.push(thread.join().unwrap());
    }
    
    // Print out what each thread returned
    println!("Intermediate sums = {:?}", intermediate_sums);

    // Sum together the return values of each thread
    let sum = reduce_data(&intermediate_sums);

    println!("Sum = {}", sum);
}

/*
*
* Partitions the data into a number of partitions such that
* - the returned partitions contain all elements that are in the input vector
* - if num_elements is a multiple of num_partitions, then all partitions must have equal number of elements
* - if num_elements is not a multiple of num_partitions, some partitions can have one more element than other partitions
*
* @param num_partitions The number of partitions to create
* @param v The data to be partitioned
* @return A vector that contains vectors of integers
* 
*/
fn partition_data(num_partitions: usize, v: &Vec<usize>) -> Vec<Vec<usize>>{

    let partition_size = v.len() / num_partitions;
    // Create a vector that will contain vectors of integers
    let mut xs: Vec<Vec<usize>> = Vec::new();

    let mut counter = 0;

    for _i in 0..num_partitions{
        // Create a vector of integers
        let mut x1 : Vec<usize> = Vec::new();

        // Add the elements to this vector
        for _j in 0..partition_size{
            x1.push(v[counter]);
            counter = counter + 1;
        }

        // Add this new vector to the return 2d vector
        xs.push(x1);
    }

    let num_remaining = v.len() - counter;

    // Push the remaining values
    for _i in 0..num_remaining{
        xs[_i].push(v[counter]);
        counter = counter + 1;
    }

    // Return the result vector
    xs
}