

//Users

val UserText = sc.textFile("./data/users.dat")

case class CUser(
    Id: Integer, 
    Sex: String, 
    Val1: Integer, 
    Val2: Integer, 
    Val3: String)

def User_Parser (linea : String) : CUser = {
   val s = linea.split("::")
   val re = CUser(s(0).toInt, 
            s(1),
            s(2).toInt,
            s(3).toInt,
            s(4))
   return re
}    

val userDF = UserText.map(s => User_Parser(s)).toDF()

userDF.take(5)
userDF.registerTempTable("users")


// Movies
val MoviesText = sc.textFile("./data/movies.dat")

case class CMovie(
    Id: Integer, 
    Title:String, 
    Genres: String)

def Movies_Parser (linea : String) : CMovie = {
   val s = linea.split("::")
   val re = CMovie(s(0).toInt, 
            s(1),
            s(2))
   return re
}    

val MovieDF = MoviesText.map(s => Movies_Parser(s)).toDF()

MovieDF.take(5)
MovieDF.registerTempTable("movies")

// Ratings
val RatingsText = sc.textFile("./data/ratings.dat")

case class CRating(
    UserId: Integer, 
    MovieId:String, 
    RatingInt: Integer,
    TimeStamp: Integer)

def Rating_Parser (linea : String) : CRating = {
   val s = linea.split("::")
   val re = CRating( s(0).toInt, 
                    s(1),
                    s(2).toInt,
                    s(3).toInt)
   return re
}    

val RatingDF = RatingsText.map(s => Rating_Parser(s)).toDF()
RatingDF.take(5)
RatingDF.registerTempTable("ratings") 

%sql
select ratingInt, count(distinct(UserId))
from ratings
where RatingInt in (select min(ratingInt) from ratings union all select max(ratingInt) from ratings)
group by ratingInt
order by ratingInt


import org.apache.spark.mllib.recommendation.ALS
import org.apache.spark.mllib.recommendation.MatrixFactorizationModel
import org.apache.spark.mllib.recommendation.Rating

// tengo que cargar de nuevo el data set pues usando este modelo de la MLLIB obliga que se usa su propia clase Rating
val data = sc.textFile("./data/ratings.dat")
val ratings_ = data.map(_.split("::") match { case Array(user, item, rate, timestamp) =>
  Rating(user.toInt, item.toInt, rate.toDouble)
})


//Usa la función randomSplit
val particion=ratings_.randomSplit(Array(0.80, 0.20))

//En el split(0) que te devuelve randomSplit tienes el RDD de training y el en 1 el de test
val training = particion(0)
val test = particion(1)

//elementos de training y test
println("Elementos en training:" + training.count())
println("Elementos en test:" + test.count())

//TIP: Para mejorar el rendimiento del proceso cachea los RDDs
training.cache()
test.cache()


// Build the recommendation model using ALS
val rank = 20
val numIterations = 10
val model = ALS.train(training, rank, numIterations, 0.01)

// Evaluate the model on rating data test and training
val usersProducts_training = training.map { case Rating(user, product, rate) =>
  (user, product)
}

usersProducts_training.take(5).foreach(print)
val predictions_training =
  model.predict(usersProducts_training).map { case Rating(user, product, rate) =>
    ((user, product), rate)
  }

predictions_training.take(5).foreach(print)

val ratesAndPreds_training = training.map { case Rating(user, product, rate) =>
  ((user, product), rate)
}.join(predictions_training)

ratesAndPreds_training.take(5).foreach(print)

val MSE_training = ratesAndPreds_training.map { case ((user, product), (r1, r2)) =>
  val err = (r1 - r2)
  err * err
}.mean()

println("Training Dataset:Mean Squared Error = " + MSE_training)

// Evaluate the model on rating data test and training
val usersProducts_test = test.map { case Rating(user, product, rate) =>
  (user, product)
}
val predictions_test =
  model.predict(usersProducts_test).map { case Rating(user, product, rate) =>
    ((user, product), rate)
  }
val ratesAndPreds_test = test.map { case Rating(user, product, rate) =>
  ((user, product), rate)
}.join(predictions_test)

val MSE_test = ratesAndPreds_test.map { case ((user, product), (r1, r2)) =>
  val err = (r1 - r2)
  print (err)
  err * err
}.mean()

println("Test Dataset:Mean Squared Error = " + MSE_test)

//show tittles function
def Show_titles (userId : Int) : Unit = {
    val recs=model.recommendProducts(userId, 5)
    val recsDF = sc.parallelize(recs).toDF()
    recsDF.createOrReplaceTempView("recs")
    val sqlDF = spark.sql("SELECT product,Title from recs inner join Movies on recs.product=Movies.Id")
    sqlDF.show()
}
//testing..
Show_titles(4169)


Show_titles: (userId: Int)Unit
+-------+--------------------+
|product|               Title|
+-------+--------------------+
|    572|Foreign Student (...|
|   2499|God Said 'Ha!' (1...|
|     59|Confessional, The...|
|   3338|For All Mankind (...|
|    681|Clean Slate (Coup...|
+-------+--------------------+


import java.io.IOException
//iter through DStream[string] and collect string, then transform it into an int
def recomend(x:Array[String]):Unit=
{
    //para cada string del array
    for(userString <- x){
        try //nos protegemos contra cosas que no sean enteros
        {
            Show_titles(userString.toInt)
        }
        catch
        {
            case ex: IOException => {
            println("conversion to int failed")
        }
    }
 
}


// Creamos un StreamingContext local con 2 threads y un intervalo batch de 5 segundos.

import org.apache.spark._
import org.apache.spark.streaming._
import scala.collection.mutable.ArrayBuffer

// creating a streming context..
val ssc = new StreamingContext(sc, Seconds(10))

val lines = ssc.socketTextStream("localhost", 9999)
val words = lines.flatMap(_.split(" "))
val arr = new ArrayBuffer[String]()

words.print()

//connecting streming with ALS recommend products
words.foreachRDD((x:RDD[String])=> recomend(x.collect()))

// start listener
ssc.start()

//in a Linux terminal, we open a netcal connection..
//nc -lk 9999
//then input some userId to get its recommendation's movies with ALS

//run this line to stop streaming
ssc.stop(stopSparkContext=false)