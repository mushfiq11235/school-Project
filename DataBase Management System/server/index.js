const express= require('express'); //routing, http requests
const cors= require('cors'); //middleware, enables scripts running on 
                             //browser client to interact with resources from different origin
const mysql= require('mysql2');
const app= express();

app.use(express.json()); //parse incoming requests to JSON
app.use(cors());

const connection= mysql.createPool({
    host: "localhost",
    user: "root",
    password: "",
    database: ""
})

//-------------------------------------------------------------------------------------------------
app.get("/", (req, res) => {
    res.send("hi");
});

app.get("/api/example", (req, res) => {
    const id= req.query.id; //eg if id=1

    const sqlget= "SELECT * FROM Recipes WHERE recipe_id= ?"; //eg attribute_name= ?
    connection.query(sqlget, [id], (err, result) => { //eg [id]=1                     select * from recipes where recipe_id=1
        res.send(result); //sends information to front end
        console.log(err); //tells us the error
    });
});

//get password
app.get("/api/password", (req,res) => {
    const id = req.query.id;
    const sqlget = " SELECT password FROM User WHERE user_id = ?";

    connection.query(sqlget, [id], (err,result) =>{
        res.send(result);
        console.log(err);
    });
});

//get preference

//set user information

//get highest ranked recipe
app.get("/api/highest_ranked", (req, res) => {
    //rating is the most
    //max()
    const sqlget = "SELECT MAX(score) as max_score, recipe_id FROM Rating GROUP BY recipe_id ORDER BY max_score DESC";
    
    connection.query(sqlget, (err,result) =>{
        res.send(result);
        console.log(err);
    });
});
//recipes 
//get all recipe names
app.get("/api/all_recipe_name", (req, res) => {
    const sqlget= "SELECT recipe_id, recipe_name FROM Recipes";

    connection.query(sqlget, (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//recipe id, recipe name, type, reference

app.get("/api/recipe_name", (req, res) => {
    const id= req.query.id;
    const sqlget= "SELECT recipe_name FROM Recipes WHERE recipe_id=?";

    connection.query(sqlget, [id], (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//get all ingredients
app.get("/api/all_recipe_ingredients", (req, res) => {
    //id, name, amount, unitprice or id, recipe id, content
    const sqlget = "SELECT recipe_id, content FROM Ingredients";

    connection.query(sqlget, (err,result) => {
        res.send(result);
        console.log(err);
    });
});

//ingredients
app.get("/api/recipe_ingredients", (req, res) => {
    //id, name, amount, unitprice or recipe id, content
    const id = req.query.id
    const sqlget = "SELECT recipe_id, content FROM Ingredients WHERE recipe_id = ?";

    connection.query(sqlget, [id], (err,result) => {
        res.send(result);
        console.log(err);
    });
});

//cooking method
app.get("/api/cookingMethod", (req, res) => {
    //recipe id, content
    const id = req.query.id;
    const sqlget= "SELECT recipe_id, content FROM CookingMethod WHERE recipe_id=?";

    connection.query(sqlget, [id], (err, result) => {
        res.send(result);
        console.log(err);
    });

});

//get all allergies 
app.get("/api/all_recipe_allergy", (req, res) => {
    //id, allergy
    const sqlget = "SELECT recipe_id, allergy FROM Recipes";

    connection.query(sqlget, (err, result) =>{
        res.send(result);
        console.log(err);
    });
});

//recipe allergy
app.get("/api/recipe_allergy", (req, res) => {
    //id, allergy
    const id = req.query.id;
    const sqlget = "SELECT recipe_id, allergy FROM Recipes WHERE recipe_id = ?";

    connection.query(sqlget, [id], (err, result) =>{
        res.send(result);
        console.log(err);
    });
});

//client allergy
app.get("/app/client_allergy", (req,res) => {
    //client_id, allergy
    const id = req.query.id
    const sqlget = "SELECT client_id, allergy FROM Client WHERE client_id = ?";

    connection.query(sqlget, [id], (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//get all countries
app.get("/api/all_recipe_country", (req, res) => {
    //id, origin_country
    const sqlget = "SELECT recipe_id, origin_country FROM Recipes";

    connection.query(sqlget, (err, result) =>{
        res.send(result);
        console.log(err);
    });
});

//origin country
app.get("/api/recipe_country", (req, res) => {
    //id, origin_country
    const id = req.query.id;
    const sqlget = "SELECT recipe_id, origin_country FROM Recipes WHERE recipe_id = ?";

    connection.query(sqlget, [id], (err, result) =>{
        res.send(result);
        console.log(err);
    });
});

//get all types
app.get("/api/all_recipe_type", (req, res) => {
    //id, type

    const sqlget= "SELECT recipe_id, type FROM Recipes";
    connection.query(sqlget, (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//recipe type
app.get("/api/recipe_type", (req, res) => {
    //id, type
    const id= req.query.id;

    const sqlget= "SELECT recipe_id, type FROM Recipes WHERE recipe_id=?";
    connection.query(sqlget, [id], (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//cooking equipment
app.get("/api/cookingEquipment", (req, res) => {
    //recipe_id, equipment_name
    const id= req.query.id;

    const sqlget= "SELECT equipment_name FROM RecipeCookingEquipment WHERE recipe_id= ?";
    connection.query(sqlget, [id], (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//get review
app.get("/api/get_recipe_review", (req, res) => {
    //comment id NOT NULL, recipe idNOT NULL, content NOT NULL, score, time NOT NULL
    const id= req.query.id;

    const sqlget= "SELECT content, score FROM RecipeComment WHERE recipe_id=?";
    connection.query(sqlget, [id], (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//get all app feedback
app.get("/api/get_all_app_feedback", (req,res) => {
    //
    const id = req.query.id;
    const sqlget = "SELECT feedback_id, content, type FROM AppFeedback"

    connection.query(sqlget, [id],(err, result) =>{
        res.send(result);
        console.log(err);
    });
});

//get app feedback by feedback_id
app.get("/api/get_app_feedback_byId", (req,res) =>{
    const id = req.query.id;
    const sqlget = "SELECT feedback_id, content, type FROM AppFeedback WHERE feedback_id = ?";

    connection.query(sqlget, [id], (err,result) =>{
        res.send(result);
        console.log(err);
    });
});

//get app feedback by type
app.get("/api/get_app_feedback_byType", (req,res) =>{
    const type = req.query.type;
    const sqlget = "SELECT feedback_id, content, type FROM AppFeedback WHERE type = ?";

    connection.query(sqlget, [type], (err,result) =>{
        res.send(result);
        console.log(err);
    });
});

//get ratings
app.get("/api/get_recipe_ratings", (req, res) => {
    //recipe id, rank, score
    const id= req.query.id;

    const sqlget= "SELECT * FROM Rating WHERE recipe_id=?";

    connection.query(sqlget, [id], (err, result) => {
        res.send(result);
        console.log(err);
    });
});

//-------------------------------------------------------------------------------------------------
//set client
app.post("/api/set_user_client", (req, res) => {
    /*
    const user_id; //req.body
    const name;
    const password;
    const email;
    const preferenece;
    const allergy;
    const sqlpost1 = "INSERT INTO User() values(?, ?, ?, ?) ";
    const sqlpost2 = "INSERT INTO Client() values(?, ?, ?)";

    connection.query(sqlpost1, [user_id, name, password, email], (err,result) => {
    });
    connection.query(sqlpost2, [user_id, Preference, allergy], (err,result) => {
    });
    */
});
//set admin
app.post("/api/set_user_admin", (req, res) =>{
    /*
    const user_id;
    const name;
    const password;
    const email;
    const job;
    const office_hour;
    const sqlpost1 = "INSERT INTO User values(?, ?, ?, ?)";
    const sqlpost2 = "INSERT INTO Admin values(?, ?, ?)";

    connection.query(sqlpost1, [user_id, name, password, email], (err, result) =>{
    });
    connection.query(sqlpost2, [user_id, job, office_hour], (err,result) => {
    });
    */
});
//set client cooking equipment
app.post("/api/set_client_cooking_equipment", (req,res) =>{
    /*
    const client_id;
    const equipment_name;
    const sqlpost = "INSERT INTO ClientCookingEquipment values(?, ?)";
    connection.query(sqlpost, [client_id, equipment_name], (err,result) =>{
        
    });
    */
});
//set recipes
app.post("/api/set_recipes", (req,res) =>{
    
    const recipe_id= req.body.id; //int 
    const recipe_name= req.body.name; //string
    const type= req.body.type; //string
    const origin_country= req.body.country; //string
    const reference= req.body.reference; //string
    const allergy= req.body.allergy; //string
    const cooking_method_content= req.body.cookmethod; //string
    const ingredients_content= req.body.ingredients; //string
    const sqlpost1 = "INSERT INTO Recipes values(?, ?, ?, ?, ?, ?)";
    const sqlpost2 = "INSERT INTO CookingMethod values(?, ?)";
    const sqlpost3 = "INSERT INTO Ingredients values(?, ?)";
    
    connection.query(sqlpost1, [recipe_id, recipe_name, type, origin_country, allergy, reference], (err,result) => {
        res.send(result);
        console.log(err); 
    });
    /*
    connection.query(sqlpost2, [recipe_id, cooking_method_content], (err,result) => {
        res.send(result);
        console.log(err); 
    });
    connection.query(sqlpost3, [recipe_id, ingredients_content], (err,result) => {
        res.send(result);
        console.log(err); 
    });
    */
});

//set recipe cooking equipment
app.post("/api/set_recipe_cooking_equipment", (req,res) =>{
    /*
    const recipe_id;
    const equipment_name;
    const sqlpost = "INSERT INTO RecipeCookingEquipment values(?, ?)";
    connection.query(sqlpost, [recipe_id, equipment_name], (err,result) =>{
        
    });
    */
});

//set app feedback
app.post("/api/set_app_feedback", (req,res) =>{
    /*
    const feedback_id;
    const content;
    const type;
    const sqlpost = "INSERT INTO AppFeedback values(?, ?, ?)";
    connection.query(sqlpost, [feedback_id, content, type], (err, result) =>{
    });
    */
});


//set review
app.post("/api/set_recipe_review", (req, res) => {
    //comment id NOT NULL, recipe idNOT NULL, content NOT NULL, score, time NOT NULL
   
   const comment_id = req.body.comment_id; 
   const recipe_id = req.body.recipe_id;
   const content = req.body.content;
   const score = req.body.score;
   const sqlpost1 = "INSERT INTO RecipeComment values(?, ?, ?, ?)";
   connection.query(sqlpost1, [comment_id, recipe_id, content, score], (err, result) =>{
    res.send(result);
    console.log(err);
   });
});

//set ratings
app.post("/api/set_recipe_ratings", (req, res) => {
    //insert into 
    const recipe_id = req.body.recipe_id;
    const pop = req.body.pop;
    const score = req.body.score;
    const sqlpost= "INSERT INTO Rating values(?, ?, ?)";
    connection.query(sqlpost, [recipe_id, pop, score], (err, result) => {
        res.send(result);
        console.log(err);
    });

});

//update ratings
app.put("/api/update_recipe_ratings", (req, res) =>{
    const recipe_id = req.body.recipe_id;
    const pop = req.body.pop;
    const score = req.body.score;
    const sqlpost = "UPDATE Rating SET popularity = ?, score = ? WHERE recipe_id = ?";
    connection.query(sqlpost, [pop, score, recipe_id], (err, result) =>{
        res.send(result);
        console.log(err);
    });
});


app.listen(7000, () =>{ //bind and listen to connections of host and port
    console.log("hello");
});